#pragma once
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <list>
#include <regex>
#include <Windows.h>

using namespace std;
class HttpResponse
{
public:
	string httpVersion;
	string returnCode;
	string returnCodeDescription;
public:
	static HttpResponse parseResponse(string resp);
};
