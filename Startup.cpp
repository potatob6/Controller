﻿#include <stdio.h>
#include <string.h>
#include <iostream>
#include "HttpResponse.h"
#include "HttpClient.h"
#pragma comment(lib, "ws2_32")

using namespace std;
int main()
{
	SetConsoleOutputCP(CP_UTF8);
	HttpClient httpClient;
	try 
	{
		//客户端启动
		httpClient.StartUpIP("127.0.0.1", 8080);
	}
	catch (int e)
	{
		if (e == -1)
			cout << u8"SOCKET已断开" << endl;
		else if (e == -2)
			cout << u8"非法端口输入" << endl;
		else if (e == -3)
			cout << u8"IP中含有非法字符" << endl;
		else if (e == -4)
			cout << u8"连接错误" << endl;
		else
			cout << u8"未知错误" << endl;
	}

	return 0;
}
