#include "MyUtils.h"
#include <iostream>
#include <string>
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
	char* TEMPSOURCE = new char[CHARLEN + 72];
	for (int i = 0; i < CHARLEN; i++)
	{
		TEMPSOURCE[i] = source[i];
	}

	int _k = 56 - (CHARLEN % 64);
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
		A = H0;
		B = H1;
		C = H2;
		D = H3;
		E = H4;

		for (int i = 0; i < 80; i++)
		{
			if (i < 16)
			{
				//小端序转为大端序
				unsigned char* FIRST = (unsigned char*)(TEMPSOURCE + i * 4 + BM);
				W[i] = ((UI)*FIRST) << 24 |
					((UI) * (FIRST + 1)) << 16 |
					((UI) * (FIRST + 2)) << 8 |
					((UI) * (FIRST + 3));
			}
			else
			{
				W[i] = MyUtils::RoundShiftLeft<UI>(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
			}
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
char MyUtils::BASE64TABLE[64] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 'T', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '-'
};
errno_t MyUtils::BASE64ENC(UC source[], UI charlen, UC out[], size_t outArySize, size_t& outEncodingSize, size_t& outADDCpos)
{
	//计算需要补全的字节
	UI ADDB = charlen % 3 == 0 ? 0 : 3 - charlen % 3;
	//计算out缓冲区是否足够大小
	if (4 * (ADDB + charlen) / 3 > outArySize)
	{
		return 1;
	}

	//处理前面 3 * (charlen / 3)的部分
	int i, j;
	i = j = 0;
	for (; i < charlen; i+=3)
	{
		if (j == 8)
		{

		}
		out[j] = (source[i] & 0b11111100) >> 2;
		out[j + 1] = ((source[i] & 0b11) << 4) | ((source[i + 1] & 0b11110000) >> 4);
		out[j + 2] = ((source[i + 1] & 0b1111) << 2) | ((source[i + 2] & 0b11000000) >> 6);
		out[j + 3] = ((source[i + 2] & 0b111111));
		j += 4;
	}
	//处理余下补全的三个字节
	if (ADDB == 1)
	{
		UI LASTJBASE = 4 * (ADDB + charlen) / 3 - 4;
		UI ADDCBASE = charlen - 2;
		out[LASTJBASE] =     (source[ADDCBASE] & 0b11111100) >> 2;
		out[LASTJBASE + 1] = ((source[ADDCBASE] & 0b11) << 4) | ((source[ADDCBASE + 1] & 0b11110000) >> 4);
		out[LASTJBASE + 2] = ((source[ADDCBASE + 1] & 0b1111) << 2);
		out[LASTJBASE + 3] = 0;
		outADDCpos = LASTJBASE + 3;
	}
	else if(ADDB == 2)
	{
		UI LASTJBASE = 4 * (ADDB + charlen) / 3 - 4;
		UI ADDCBASE = charlen - 1;
		out[LASTJBASE] = (source[ADDCBASE] & 0b11111100) >> 2;
		out[LASTJBASE + 1] = ((source[ADDCBASE] & 0b11) << 4);
		out[LASTJBASE + 2] = 0;
		out[LASTJBASE + 3] = 0;
		outADDCpos = LASTJBASE + 2;
	}
	outEncodingSize = 4 * (ADDB + charlen) / 3;
	return 0;
}
using namespace std;
string MyUtils::BASE64ENCSTR(UC source[])
{
	UI len = strlen((char*)source);
	return MyUtils::BASE64ENCSTR(source, len);
}

string MyUtils::BASE64ENCSTR(UC source[], UI sourceLength)
{
	UC* u = new UC[sourceLength * 4 / 3 + 3];
	string TEMP;
	UI offLength, emptyLen;
	MyUtils::BASE64ENC(source, sourceLength, u, sourceLength * 4 / 3 + 3, offLength, emptyLen);
	for (int i = 0; i < offLength; i++)
	{
		if (i < emptyLen)
		{
			TEMP.push_back(MyUtils::BASE64TABLE[u[i]]);
		}
		else
		{
			TEMP.push_back('=');
		}
	}
	delete[] u;
	return TEMP;
}

errno_t BASE64DECTABLE(UC c, UC& out)
{
	if (c >= (UC)'A' && c <= (UC)'Z')
	{
		out = c - (UC)'A';
		return 0;
	}
	if (c >= (UC)'a' && c <= (UC)'z')
	{
		out = c - (UC)'a' + 26;
		return 0;
	}
	if (c >= (UC)'0' && c <= (UC)'9')
	{
		out = c - (UC)'0' + 52;
		return 0;
	}

	switch (c)
	{
		case (UC)'+':
			out = 62;
			return 0;
		case (UC)'-':
			out = 63;
			return 0;
		case (UC)'=':
			out = 0;
			return 0;
		default:
			return 1;
	}
	return 1;
}

errno_t MyUtils::BASE64DECSTR(string source, char* dest, size_t destSize, size_t& outSize)
{
	if (source.size() % 4 != 0)
	{
		return 1;
	}
	//检查dest长度是否足够
	if (destSize < source.size() * 3 / 4)
	{
		return 2;
	}
	UI ADDC = 0;
	UC* TEMP = new UC[source.size()];

	auto itor = source.begin();
	int i = 0;
	bool success = true;
	for (; itor != source.end(); itor++)
	{
		UC READB;
		errno_t ret = BASE64DECTABLE((UC)*itor, READB);
		if (ret == 1)
		{
			success = false;
			break;
		}
		TEMP[i] = READB;
		if (*itor == '=')
		{
			ADDC++;
		}
		i++;
	}
	if (!success)
	{
		delete[] TEMP;
		return 1;
	}

	outSize = source.size() * 3 / 4 - ADDC;

	i = 0;
	int j = 0;
	for (; i < source.size(); i+=4)
	{
		dest[j] = ((TEMP[i] & 0b111111) << 2) | ((TEMP[i + 1] & 0b110000) >> 4);
		dest[j + 1] = ((TEMP[i + 1] & 0b1111) << 4) | ((TEMP[i + 2] & 0b111100) >> 2);
		dest[j + 2] = ((TEMP[i + 2] & 0b11) << 6) | (TEMP[i + 3] & 0b111111);
		j+=3;
	}

	delete[] TEMP;
	return 0;
}

string MyUtils::ToHexDigest(UC* s, size_t slen)
{
	string n;
	for (int i = 0; i < slen; i++)
	{
		char t[3] = {0};
		sprintf_s(t, 3, "%02x", s[i]);
		n.append(t);
	}
	return n;
}

int MyUtils::GETFIN(WebSocketHeader* header)
{
	return (header -> FRRROPCODE & 0b10000000) >> 7;
}

int MyUtils::GETRESV(WebSocketHeader* header, int n)
{
	if (n == 1)
		return (header->FRRROPCODE & 0b01000000) >> 6;
	if (n == 2)
		return (header->FRRROPCODE & 0b00100000) >> 5;
	if (n == 3)
		return (header->FRRROPCODE & 0b00010000) >> 5;
}

UI MyUtils::GETOPCODE(WebSocketHeader* header)
{
	return header->FRRROPCODE & 0b1111;
}

UC MyUtils::GETPAYLOADLEN(WebSocketHeader* header)
{
	return header->PAYLOADLEN;
}