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
}