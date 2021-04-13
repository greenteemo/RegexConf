# 编译动态库
g++ RegexConf.cpp -o RegexConf.o -c -fPIC -lpthread -I../include
g++ RegexConf.o -shared -o libregexconf.so -lpthread
mv libregexconf.so ../lib
rm RegexConf.o

# 编译获取版本号BIN文件
g++ RegexConfVersion.cpp -o RegexConfVersion -I../include
mv RegexConfVersion ../lib
