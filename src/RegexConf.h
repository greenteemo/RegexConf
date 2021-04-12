#include <stdio.h>
#include <regex.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unordered_map>

using namespace std;

#define RET_SUCCESS 0
#define RET_FAILURE 1
#define CONF_MAX_LINE_NUM 256


class RegexConf
{
private:
	regex_t m_regex;
	regmatch_t *m_regexmatch = NULL;
	string m_conf_filename;
	unsigned int m_conf_last_modified_time = 0;
	pthread_t m_detect_conf_thread_handle;
	pthread_rwlock_t m_rwlock;
	string m_regex_pattern;
	unordered_map<string, string> m_conf_key_value;
	
	string GetRegexmatch(size_t pos, char *line);

public:
	RegexConf();
	RegexConf(const string filename, const string pattern);
	~RegexConf();

	int SetRegexPattern(const string pattern);
	int SetConfFilename(const string filename);
	const string GetConfFilename();
	void SetConfLastModifiedTime(unsigned int conf_last_modified_time);
	const unsigned int GetConfLastModifiedTime();
	int GetAllConf();
	string GetConfValue(const string key);
};


void* detectConf(void *self);
unsigned int getFileLastModifiedTime(const string filename);

