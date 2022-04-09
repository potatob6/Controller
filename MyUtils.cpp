#include "MyUtils.h"
#include <iostream>
using namespace pb666;
void MyUtils::pushStringToList(string n, list<char>& chars)
{
	auto itor = n.begin();
	for (; itor != n.end(); itor++)
	{
		chars.push_back(*itor);
	}
}

template<typename T>
static T MyUtils::RoundShiftLeft(T c, UI n)
{
	int TSIZE = sizeof(T);
	return (c << (n % (TSIZE * 8))) | (c >> (TSIZE * 8 - (n % (TSIZE * 8))));
}
namespace pb666 {

	int F(int t, int B, int C, int D)
	{
		if (t >= 60)
			return B ^ C ^ D;
		else if (t >= 40)
			return (B & C) | (B & D) | (C & D);
		else if (t >= 20)
			return B ^ C ^ D;
		else
			return (B & C) | ((~B) & D);
	}
	int K(int t)
	{
		if (t >= 60)
			return 0xCA62C1D6;
		else if (t >= 40)
			return 0x8F1BBCDC;
		else if (t >= 20)
			return 0x6ED9EBA1;
		else
			return 0x5A827999;
	}
}
void MyUtils::SHA1(char* source, ULL CHARLEN, char out[20])
{
	char* TEMPSOURCE = new char[CHARLEN + 64];
	for (int i = 0; i < CHARLEN; i++)
	{
		TEMPSOURCE[i] = source[i];
	}

	UI _k = 56 - (CHARLEN % 64);
	UI k = _k > 0 ? _k : (_k == 0 ? 64 : 64 + _k);
	//补位
	for (int i = 0; i < k; i++)
	{
		if (i == 0)
		{
			TEMPSOURCE[CHARLEN + i] = 0b10000000;
			continue;
		}
		TEMPSOURCE[CHARLEN + i] = 0x0;
	}


	//补长度
	ULL BITLEN = (ULL)CHARLEN * 8;
	ULL CP_LEN = CHARLEN + k + 8;
	TEMPSOURCE[CP_LEN - 1] =  (BITLEN) & 0xff;
	TEMPSOURCE[CP_LEN - 2] = ((BITLEN) >> 8) & 0xff;
	TEMPSOURCE[CP_LEN - 3] = ((BITLEN) >> 16) & 0xff;
	TEMPSOURCE[CP_LEN - 4] = ((BITLEN) >> 24) & 0xff;
	TEMPSOURCE[CP_LEN - 5] = ((BITLEN) >> 32) & 0xff;
	TEMPSOURCE[CP_LEN - 6] = ((BITLEN) >> 40) & 0xff;
	TEMPSOURCE[CP_LEN - 7] = ((BITLEN) >> 48) & 0xff;
	TEMPSOURCE[CP_LEN - 8] = ((BITLEN) >> 56) & 0xff;


	//计算消息摘要

	UI A, B, C, D, E;
	UI H0, H1, H2, H3, H4;
	UI* W = new UI[80];
	H0 = 0x67452301;
	H1 = 0xEFCDAB89;
	H2 = 0x98BADCFE;
	H3 = 0x10325476;
	H4 = 0xC3D2E1F0;

	for (int Mi = 0; Mi < CP_LEN / 64; Mi++)
	{
		UI BM = Mi * 64;
		//TODO
		for (int i = 0; i < 16; i++)
		{
			//小端序转为大端序
			unsigned char* FIRST = (unsigned char*)(TEMPSOURCE + i * 4 + BM);
			W[i] = ((UI)*FIRST) << 24 |
				((UI)*(FIRST+1)) << 16 |
				((UI)*(FIRST+2)) << 8 |
				((UI)*(FIRST+3));
		}
		for (int i = 16; i < 80; i++)
			W[i] = MyUtils::RoundShiftLeft<UI>(W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16], 1);
		A = H0;
		B = H1;
		C = H2;
		D = H3;
		E = H4;
		for (int i = 0; i < 80; i++)
		{
			UI TEMP = MyUtils::RoundShiftLeft<UI>(A, 5) +
				pb666::F(i, B, C, D) +
				E +
				W[i] +
				pb666::K(i);
			E = D;
			D = C;
			C = MyUtils::RoundShiftLeft<UI>(B, 30);
			B = A;
			A = TEMP;
		}
		H0 = H0 + A;
		H1 = H1 + B;
		H2 = H2 + C;
		H3 = H3 + D;
		H4 = H4 + E;
	}


	//处理完成
	out[0] = (H0 & 0xff000000) >> 24;
	out[1] = (H0 & 0xff0000) >> 16;
	out[2] = (H0 & 0xff00) >> 8;
	out[3] = H0 & 0xff;

	out[4] = (H1 & 0xff000000) >> 24;
	out[5] = (H1 & 0xff0000) >> 16;
	out[6] = (H1 & 0xff00) >> 8;
	out[7] = H1 & 0xff;

	out[8]  = (H2 & 0xff000000) >> 24;
	out[9]  = (H2 & 0xff0000) >> 16;
	out[10] = (H2 & 0xff00) >> 8;
	out[11] = H2 & 0xff;

	out[12] = (H3 & 0xff000000) >> 24;
	out[13] = (H3 & 0xff0000) >> 16;
	out[14] = (H3 & 0xff00) >> 8;
	out[15] = H3 & 0xff;

	out[16] = (H4 & 0xff000000) >> 24;
	out[17] = (H4 & 0xff0000) >> 16;
	out[18] = (H4 & 0xff00) >> 8;
	out[19] = H4 & 0xff;

	delete[] W;
	delete[] TEMPSOURCE;
}