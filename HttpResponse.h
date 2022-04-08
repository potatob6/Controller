#pragma once
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <list>
#include <regex>
#include <iostream>
#include <Windows.h>
#include <string>
#include "GlobalParameters.h"


using namespace std;
using namespace pb666;
class HttpResponse
{
public:
	string httpVersion;
	string returnCode;
	string returnCodeDescription;
	list<pair<string, string>> attributes;
public:
	static HttpResponse parseResponse(string resp);
	HttpResponse();
	HttpResponse(const HttpResponse&);
	bool getPair(string, SSP*);               //遍历查找对应的Key
	string toString();
};
