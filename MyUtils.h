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
	static void SHA1(char* source, ULL CHARLEN, char out[20]);

	template<typename T>
	static inline T RoundShiftLeft(T, UI);
	static char     BASE64TABLE[64];
	static errno_t  BASE64ENC(UC source[], UI charlen, UC out[], size_t outArySize, size_t& outEncodingSize, size_t& outADDCpos);
	static string   BASE64ENCSTR(UC[]);
	static string   BASE64ENCSTR(UC[], UI sourceLength);
	static errno_t  BASE64DECSTR(string source, char* dest, size_t destSize, size_t& outSize);
	static string   ToHexDigest(UC* s, size_t slen);
	static int      GETFIN(WebSocketHeader* header);
	static int      GETRESV(WebSocketHeader* header, int n);
	static UI       GETOPCODE(WebSocketHeader* header);
	static UC       GETPAYLOADLEN(WebSocketHeader* header);

};

errno_t BASE64DECTABLE(UC c, UC& out);