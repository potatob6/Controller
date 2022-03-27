#pragma once
#include <string>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <list>
#include <regex>
#include <Windows.h>
using namespace std;

class HttpClient {
private:
	char* _recv_buf;
	unsigned long byte_pointer = 0;
	unsigned long total_recv = 0;
	unsigned long current_stream_length = 0;
	unsigned char getRnRn = 0;            // end of head bytes \r \n \r \n
	unsigned char getRn0Rn = 0;           // end of chuncked   \r \n 0 \r \n
	unsigned char enterEscapeMode = 0;
	char getNextByte();

	//read next byte and judge is this stream read end of http response
	bool judgeRn0Rn(char nb);

	//read next byte and judge is this stream read end of http head
	bool judgeRnRn(char nb);
public:
	SOCKET server;
	string ip;
	string port;

	string Send(string method, string url, string content);
	string Send(string method, string url, map<string, string> extraAttribute, string content);

	string ReceiveHead();

	HttpClient(SOCKET socket);
	~HttpClient();
};