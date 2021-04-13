export LD_LIBRARY_PATH=../lib:${LD_LIBRARY_PATH}

rm -f a.out
g++ main.cpp -L../lib -lregexconf -I../include

../lib/RegexConfVersion
./a.out
