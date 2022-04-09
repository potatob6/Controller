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
	static void PrintBits(T num)
	{
		int bitlen = 8 * sizeof(T);
		for (int i = 1; i <= bitlen; i++)
		{
			printf("%d", (num >> (bitlen - i)) & 0x1);
			if (i % 8 == 0)
			{
				printf(" ");
			}
		}
	}
	template<typename T>
	static inline T RoundShiftLeft(T, UI);
	static char BASE64TABLE[64];
	static errno_t BASE64ENC(UC[], UI, UC[], size_t, size_t&, size_t&);
};
