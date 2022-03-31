#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <Windows.h>
#include "GlobalParameters.h"
#include "HttpResponse.h"
#include "HttpClient.h"
#pragma comment(lib, "ws2_32")

using namespace std;
int main()
{
	SetConsoleOutputCP(CP_UTF8);
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr.S_un.S_addr);
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int CR = connect(server, (const sockaddr*)&addr, sizeof(addr));
	if (CR == SOCKET_ERROR) {
		printf_s(u8"连接失败");
		return 1;
	}
	
	string content1 = u8"";
	HttpClient httpClient(server);
	httpClient.ip = "127.0.0.1";
	httpClient.port = "8080";

	try
	{
		map<string, string> mm;
		string sendStr = httpClient.Send("GET", "/JSP/", content1);

#ifdef DEBUG_MODE
		cout << u8"本地发送的消息" << endl;
		cout << sendStr;
		cout << u8"end" << endl;
#endif
	}
	catch (int e)
	{
		cout << u8"发送失败" << endl;
	}

	try 
	{
		string resp = httpClient.ReceiveHead();

#ifdef DEBUG_MODE
		cout << u8"服务器返回头部" << endl;
		cout << resp;
#endif

		HttpResponse::parseResponse(resp);
	}
	catch (int e)
	{
#ifdef DEBUG_MODE
		cout << u8"解析头部失败" << endl;
#endif
	}

	closesocket(server);

	WSACleanup();

	return 0;
}
