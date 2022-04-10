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

	//一次WebSocket握手协议的BASE64编解码与SHA1
	char* t = (char*)"zsclVTq+juB3I2dETaPNnw==";
	UC* decbuf = new UC[strlen(t) * 3 / 4];
	UC* buf = new UC[20];

	size_t outsize;
	errno_t ret = MyUtils::BASE64DECSTR(t, (char*)decbuf, strlen(t) * 3 / 4, outsize);
	if (ret == 0)
	{
		MyUtils::SHA1((char*)decbuf, outsize, (char*)buf);
		cout << "SHA1:" + MyUtils::ToHexDigest(buf, 20) << endl;
		cout << "BASE64:" + MyUtils::BASE64ENCSTR(buf, 20) << endl;
	}
	
	delete[] buf, decbuf;
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
