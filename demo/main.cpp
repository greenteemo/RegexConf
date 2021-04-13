#include <stdio.h>
#include <string>
#include "RegexConf.h"


int main()
{
	printf("\n##### Test RegexConf #####\n");
	
	RegexConf *regexConf = new RegexConf("./test.conf", "^[ ]*([^#\n]*?)[ ]*=[ ]*([^\n ]*?)[ ]*$");
	printf("a:[%s]\n", regexConf->GetConfValue("a").c_str());
	printf("b:[%s]\n", regexConf->GetConfValue("b").c_str());


	printf("\n# 修改正则表达式匹配模式（配置项和对应值由等号分隔改为冒号分隔）\n");
	const std::string pattern = "^[ ]*([^#\n]*?)[ ]*:[ ]*([^\n ]*?)[ ]*$";
	regexConf->SetRegexPattern(pattern);
	printf("a:[%s]\n", regexConf->GetConfValue("a").c_str());
        printf("b:[%s]\n", regexConf->GetConfValue("b").c_str());

	printf("\n# 更换配置文件为 test.conf2\n");
        regexConf->SetConfFilename("./test.conf2");
        printf("a:[%s]\n", regexConf->GetConfValue("a").c_str());
        printf("b:[%s]\n", regexConf->GetConfValue("b").c_str());

	printf("\n# 手动输入配置项获取对应配置值（可尝试修改配置文件，查看配置是否实时更新）\n");
	while (1)
	{
		char key[256] = {0};
		printf("Get conf value of:");
		scanf("%s", key);
		printf("%s\n", regexConf->GetConfValue(key).c_str());
	}
	
	return 0;
}
