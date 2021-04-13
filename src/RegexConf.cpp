#include "RegexConf.h"


RegexConf::RegexConf()
{

}


RegexConf::RegexConf(const string filename, const string pattern)
{
	if (RET_SUCCESS != SetConfFilename(filename) && RET_SUCCESS != SetRegexPattern(pattern))
	{
		fprintf(stderr, "New RegexConf failed!\n");
		return;
	}
}


RegexConf::~RegexConf()
{
	if (m_detect_conf_thread_handle != 0)
        {
                int ret = pthread_kill(m_detect_conf_thread_handle, 0);
                if (ret == 0)
                {
                        pthread_cancel(m_detect_conf_thread_handle);
                        pthread_join(m_detect_conf_thread_handle, NULL);
                }
        }
}


int RegexConf::SetRegexPattern(const string pattern)
{
	m_regex_pattern = pattern;

	char errbuff[256];
	int ret;

	ret = regcomp(&m_regex, m_regex_pattern.c_str(), REG_EXTENDED|REG_NEWLINE);
	if (ret)
	{
		regerror(ret, &m_regex, errbuff, sizeof(errbuff));
		fprintf(stderr, "[RegexConf] ERROR STRING:[%s] ERROR REASON:[%s]\n", m_regex_pattern.c_str(), errbuff);
		return RET_FAILURE;
	}
	m_regexmatch = (regmatch_t*)malloc((m_regex.re_nsub+1) * sizeof(regmatch_t));

	return GetAllConf();
}


int RegexConf::SetConfFilename(const string filename)
{
	// 可能存在多次指定配置文件，需要把之前创建的线程关闭。
	if (m_detect_conf_thread_handle != 0)
	{	
		int ret = pthread_kill(m_detect_conf_thread_handle, 0);
		if (ret == 0)
		{
			pthread_cancel(m_detect_conf_thread_handle);
			pthread_join(m_detect_conf_thread_handle, NULL);
		}
	}

	m_conf_filename = filename;	
	m_conf_last_modified_time = getFileLastModifiedTime(m_conf_filename);
	if (m_conf_last_modified_time == 0)
	{
		fprintf(stderr, "[RegexConf] Cannot get file [%s] last modified time!\n", m_conf_filename.c_str());
                return RET_FAILURE;
	}

	pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
        pthread_attr_setstacksize(&attr, 40 * 1024 * 1024);

        if (pthread_create(&m_detect_conf_thread_handle, &attr, detectConf, (void*)this) != 0)
        {
                fprintf(stderr, "[RegexConf] Failed to create detect config thread handle!\n");
                return EXIT_FAILURE;
        }
	
	return GetAllConf();
}


const string RegexConf::GetConfFilename()
{
	return m_conf_filename;
}


void RegexConf::SetConfLastModifiedTime(unsigned int conf_last_modified_time)
{
	m_conf_last_modified_time = conf_last_modified_time;
}


const unsigned int RegexConf::GetConfLastModifiedTime()
{
	return m_conf_last_modified_time;
}


int RegexConf::GetAllConf()
{
	if (m_regexmatch == NULL)
		return RET_FAILURE;

	FILE *fp_conf = NULL;
	if ((fp_conf = fopen(m_conf_filename.c_str(), "r")) == NULL)
	{
		fprintf(stderr, "[RegexConf] Cannot open config file [%s]!", m_conf_filename.c_str());
		return RET_FAILURE;
	}

	pthread_rwlock_wrlock(&m_rwlock);

	char line[CONF_MAX_LINE_NUM] = {0};
	char errbuff[256];
	m_conf_key_value.clear();
	while (fgets(line, CONF_MAX_LINE_NUM, fp_conf) != NULL)
	{
		int ret;
		ret = regexec(&m_regex, line, m_regex.re_nsub+1, m_regexmatch, 0);
		if(ret)
		{
			continue;	// 不匹配，跳过改行
		}

		string key = GetRegexmatch(1, line);
		string value = GetRegexmatch(2, line);
		m_conf_key_value.insert(make_pair(key, value));
	}

	pthread_rwlock_unlock(&m_rwlock);
	
	return RET_SUCCESS;	
}


string RegexConf::GetConfValue(const string key)
{
	pthread_rwlock_rdlock(&m_rwlock);	
	string value = m_conf_key_value[key];
	pthread_rwlock_unlock(&m_rwlock);

	return value;
}


string RegexConf::GetRegexmatch(size_t pos, char *line)
{
	int len = m_regexmatch[pos].rm_eo - m_regexmatch[pos].rm_so;
	char *temp = (char*)malloc(len + 1);
	strncpy(temp, line + m_regexmatch[pos].rm_so,len);
	temp[len] = 0;
	return temp;
}


void* detectConf(void *self)
{
	RegexConf *regexConf = (RegexConf*)self;
	while (1)
	{
		unsigned int curr_conf_last_modified_time = getFileLastModifiedTime(regexConf->GetConfFilename());
		if (regexConf->GetConfLastModifiedTime() != curr_conf_last_modified_time)
		{
			regexConf->SetConfLastModifiedTime(curr_conf_last_modified_time);	
			regexConf->GetAllConf();
		}
		sleep(1);
	}
	return NULL;	
}


unsigned int getFileLastModifiedTime(const string filename)
{
	struct stat buf;
	int result;

	result =stat( filename.c_str(), &buf );
	if( result != 0 )
		perror( "Cannot get file last modified time!" );

	return buf.st_mtime;
}
