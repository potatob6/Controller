#pragma once
#include "GlobalParameters.h"
#include <string>
#include <list>
using namespace std;
using namespace pb666;
class MyUtils
{
public:
	static void pushStringToList(string n, list<char>& chars);
	static inline void setBit(char*, ULL, ULL);   //设置比特位
	static inline char getBit(char*, ULL, ULL);  //获得比特位
	static void sha1(char*, ULL, char*);
};

