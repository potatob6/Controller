#include <stdio.h>
#include <string.h>
#include <iostream>
#include "HttpResponse.h"
#include "HttpClient.h"
#include "MyUtils.h"
#include "GlobalParameters.h"
#pragma comment(lib, "ws2_32")

using namespace std;
using namespace pb666;



int main()
{
	SetConsoleOutputCP(CP_UTF8);
	char* t = (char*)"zsclVTq+juB3I2dETaPNnw==";
	UC* buf = new UC[20];
	MyUtils::SHA1(t, strlen(t), (char*)buf);
	for (int i = 0; i < 20; i++)
	{
		printf("%02x", (unsigned char)buf[i]);
	}
	cout << endl;
	cout << MyUtils::BASE64ENCSTR((UC*)"fuckinglove") << endl;
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
