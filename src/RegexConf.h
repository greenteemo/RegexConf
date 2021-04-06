#include <stdio.h>
#include <regex.h>
#include <sys/types.h>
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
        regmatch_t *m_regexmatch;
	string m_conf_filename;
	string m_regex_pattern;
	unordered_map<string, string> m_conf_key_value;
	
	string GetRegexmatch(size_t pos, char *line);
	int GetAllConf();

public:
        RegexConf();
        RegexConf(const string filename, const string pattern);
        ~RegexConf();

	int SetRegexPattern(const string pattern);
	int SetConfFilename(const string filename);
	string GetConfValue(const string key);
};
