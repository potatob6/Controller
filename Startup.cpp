#include <stdio.h>
#include <string.h>
#include <iostream>
#include "HttpResponse.h"
#include "HttpClient.h"
#include "MyUtils.h"
#pragma comment(lib, "ws2_32")

using namespace std;
int main()
{
	SetConsoleOutputCP(CP_UTF8);
	char* t = (char*)"illll";
	char* buf = new char[20];
	MyUtils::sha1(t, 5, buf);
	cout << buf << endl;
	delete[] buf;
	return 0;
	try
	{
		//客户端启动
		HttpClient httpClient;
		httpClient.StartUpIP("127.0.0.1", 8080);
	}
	catch (int e)
	{
		if (e == -1)
			cout << u8"服务器已关闭" << endl;
		else if (e == -2)
			cout << u8"非法端口输入" << endl;
		else if (e == -3)
			cout << u8"IP中含有非法字符" << endl;
		else if (e == -4)
			cout << u8"连接错误" << endl;
		else if (e == -5)
			cout << u8"SOCKET异常" << endl;
		else if (e == -6)
			cout << u8"发送失败" << endl;
	}
	return 0;
}
