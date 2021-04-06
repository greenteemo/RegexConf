#include "RegexConf.h"


RegexConf::RegexConf()
{
	m_regexmatch = NULL;
}


RegexConf::RegexConf(const string filename, const string pattern)
{
	m_regexmatch = NULL;
	if (RET_SUCCESS != SetConfFilename(filename) && RET_SUCCESS != SetRegexPattern(pattern))
	{
		fprintf(stderr, "New RegexConf failed!\n");
		return;
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
	m_conf_filename = filename;
	return GetAllConf();
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

        char line[CONF_MAX_LINE_NUM] = {0};
	char errbuff[256];
	while (fgets(line, CONF_MAX_LINE_NUM, fp_conf) != NULL)
        {
        	int ret;
        	ret = regexec(&m_regex, line, m_regex.re_nsub+1, m_regexmatch, 0);
      		if(ret)
       		{
                	regerror(ret, &m_regex, errbuff, sizeof(errbuff));
                	printf("[RegexConf] ERROR PATTERN:[%s] ERROR STRING:[%s] ERROR REASON:[%s]\n", m_regex_pattern, line, errbuff);
                	return RET_FAILURE;
       		}

                string key = GetRegexmatch(1, line);
		string value = GetRegexmatch(2, line);
               	m_conf_key_value.insert(make_pair(key, value));
        }
	
	return RET_SUCCESS;	
}


string RegexConf::GetConfValue(const string key)
{
	return m_conf_key_value[key];
}


string RegexConf::GetRegexmatch(size_t pos, char *line)
{
	int len = m_regexmatch[pos].rm_eo - m_regexmatch[pos].rm_so;
        char *temp = (char*)malloc(len + 1);
        strncpy(temp, line + m_regexmatch[pos].rm_so,len);
        temp[len] = 0;
        return temp;
}


int main()
{
	RegexConf *regexConf = new RegexConf("./test.conf", "^[ ]*([^#\n]*?)[ ]*=[ ]*([^\n ]*?)[ ]*$");
	
	printf("a:[%s]\n", regexConf->GetConfValue("a").c_str());
	printf("b:[%s]\n", regexConf->GetConfValue("b").c_str());
	printf("c:[%s]\n", regexConf->GetConfValue("c").c_str());
	printf("d:[%s]\n", regexConf->GetConfValue("d").c_str());

	return 0;
}
