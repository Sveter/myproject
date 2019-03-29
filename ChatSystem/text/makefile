all: jsontest
jsontest:jsontest.cc
	g++ -o $@ $^ -I./lib/include -L./lib/lib -std=c++11 -ljsoncpp
