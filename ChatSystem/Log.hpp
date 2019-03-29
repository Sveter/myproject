#pragma once

#include <iostream>
#include <string>

#define NORMAL 0
#define WARNING 1
#define ERROR 2

const char* getLevel[] = {
	"Normal",
	"Warning",
	"Error",
	NULL
};

void Log(std::string msg, int level, const char *file, int line){
	std::cout << msg << "[" << getLevel[level] << "]" << " : " << "file: " << file << ", line: " << line << std::endl;
}

#define LOG(msg, level) Log(msg, level, __FILE__, __LINE__)
