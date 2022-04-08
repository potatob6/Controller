#include "MyUtils.h"

using namespace pb666;
void MyUtils::pushStringToList(string n, list<char>& chars)
{
	auto itor = n.begin();
	for (; itor != n.end(); itor++)
	{
		chars.push_back(*itor);
	}
}

inline void MyUtils::setBit(char* source, 
	ULL CHARLEN, 
	ULL POSI)
{
	//TODO
}

inline char MyUtils::getBit(char* source, 
	ULL CHARLEN, 
	ULL POSI)
{
	//TODO
	UI chars = POSI / 8;

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
void MyUtils::sha1(char* source, ULL CHARLEN, char out[20])
{
	char* TEMPSOURCE = new char[CHARLEN + 64];
	for (int i = 0; i < CHARLEN; i++)
	{
		TEMPSOURCE[i] = source[i];
	}

	unsigned int _k = 52 - (CHARLEN % 64);
	unsigned int k = _k > 0 ? _k : (_k == 0 ? 64 : 64 + _k);
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
	unsigned long long CP_LEN = CHARLEN + k;
	TEMPSOURCE[CP_LEN] = (CHARLEN * 8) & 0xff;
	TEMPSOURCE[CP_LEN - 1] = (((UI)CHARLEN * 8) >> 8) & 0xff;
	TEMPSOURCE[CP_LEN - 2] = (((UI)CHARLEN * 8) >> 16) & 0xff;
	TEMPSOURCE[CP_LEN - 3] = (((UI)CHARLEN * 8) >> 24) & 0xff;
	TEMPSOURCE[CP_LEN - 4] = (((UI)CHARLEN * 8) >> 32) & 0xff;
	TEMPSOURCE[CP_LEN - 5] = (((UI)CHARLEN * 8) >> 40) & 0xff;
	TEMPSOURCE[CP_LEN - 6] = (((UI)CHARLEN * 8) >> 48) & 0xff;
	TEMPSOURCE[CP_LEN - 7] = (((UI)CHARLEN * 8) >> 56) & 0xff;

	//计算消息摘要

	int A, B, C, D, E;
	int H0, H1, H2, H3, H4;
	int* W = new int[80];
	H0 = 0x67452301;
	H1 = 0xEFCDAB89;
	H2 = 0x98BADCFE;
	H3 = 0x10325476;
	H4 = 0xC3D2E1F0;

	for (int Mi = 0; Mi < CP_LEN; Mi++)
	{
		UI BM = Mi * 64;
		//TODO
		for (int i = 0; i < 15; i++)
			W[i] = *(int*)(TEMPSOURCE + i * 4 + BM);
		for (int i = 16; i < 80; i++)
			W[i] = ((UI)(W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16])) << 1;
		A = H0;
		B = H1;
		C = H2;
		D = H3;
		E = H4;
		for (int i = 0; i < 80; i++)
		{
			int TEMP = ((UI)A) << 5 +
				pb666::F(i, B, C, D) +
				E +
				W[i] +
				pb666::K(i);
			E = D;
			D = C;
			C = ((UI)B) << 30;
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
	UI UIH0 = (UI)H0;
	out[0] = UIH0 & 0xff;
	out[1] = (UIH0 >> 8) & 0xff;
	out[2] = (UIH0 >> 16) & 0xff;
	out[3] = (UIH0 >> 24) & 0xff;

	UI UIH1 = (UI)H1;
	out[4] = UIH1 & 0xff;
	out[5] = (UIH1 >> 8) & 0xff;
	out[6] = (UIH1 >> 16) & 0xff;
	out[7] = (UIH1 >> 24) & 0xff;

	UI UIH2 = (UI)H2;
	out[8] = UIH0 & 0xff;
	out[9] = (UIH0 >> 8) & 0xff;
	out[10] = (UIH0 >> 16) & 0xff;
	out[11] = (UIH0 >> 24) & 0xff;

	UI UIH3 = (UI)H3;
	out[12] = UIH3 & 0xff;
	out[13] = (UIH3 >> 8) & 0xff;
	out[14] = (UIH3 >> 16) & 0xff;
	out[15] = (UIH3 >> 24) & 0xff;

	UI UIH4 = (UI)H4;
	out[16] = UIH4 & 0xff;
	out[17] = (UIH4 >> 8) & 0xff;
	out[18] = (UIH4 >> 16) & 0xff;
	out[19] = (UIH4 >> 24) & 0xff;

	delete[] W;
	delete[] TEMPSOURCE;
}