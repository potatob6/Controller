#pragma once
#include <string>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <list>
#include <regex>
#include <Windows.h>
using namespace std;
class HttpRequest {
public:
	string _method;
	string _url;
	string _httpVersion;
	map<string, string> _requestHead;
	string _requestBody;

	string getHeaderStr(string key);
	map<string, string>::iterator getHeaderKeypair(string key);
	string toString();

	void setAttribute(string key, string value);
	unsigned long getContentLength();
};