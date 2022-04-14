#pragma once
#define DEBUG_MODE
#define RETRY_TIMES 2
#define HTTPCLIENT_RETRY_DELAY 1000

#include <map>
#include <string>

using namespace std;
namespace pb666 {
	typedef pair<string, string> SSP;
	typedef unsigned long long ULL;
	typedef unsigned int UI;
	typedef long long LL;
	typedef map<string, string> MSS;
	typedef unsigned char UC;
}

using namespace pb666;
#pragma pack(1)
typedef struct
{
	UC       FRRROPCODE;
	UC       PAYLOADLEN;
	//uint16_t EXTPAYLOAD;          //这两个在WebSocketClient中根据PAYLOADLEN获取
	//uint32_t EXTPAYLOADCONTINUE;
	//TODO
}WebSocketHeader;
#pragma pack()