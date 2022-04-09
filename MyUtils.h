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
	static void SHA1(char*, ULL, char*);

	template<typename T>
	static inline T RoundShiftLeft(T, UI);
	static char BASE64TABLE[64];
	static errno_t BASE64ENC(UC[], UI, UC[], size_t, size_t&, size_t&);
	static string BASE64ENCSTR(UC[]);
	static string BASE64ENCSTR(UC[], UI sourceLength);
};
