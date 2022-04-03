#include "HttpClient.h"
#include "HttpRequest.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <iostream>
#include "GlobalParameters.h"
#include "HttpResponse.h"

using namespace std;
char HttpClient::getNextByte()
{
	if (byte_pointer == total_recv)
	{
		//read from socket
		total_recv = recv(server, _recv_buf, 1024 * 1024, 0);
		if (total_recv <= 0)
		{
			throw - 1;
		}
		_recv_buf[total_recv] = 0;
		byte_pointer = 0;
	}
	char _temp = _recv_buf[byte_pointer];
	byte_pointer++;
	current_stream_length++;
	return _temp;
}

bool HttpClient::judgeRn0Rn(char nb)
{
	if (nb == '\r' && (getRn0Rn == 0 || getRn0Rn == 3))
	{
		getRn0Rn++;
		return false;
	}
	else
	{
		getRn0Rn = 0;
	}

	if (nb == '0' && getRn0Rn == 2)
	{
		getRn0Rn++;
		return false;
	}
	else
	{
		getRn0Rn = 0;
	}

	if (nb == '\n' && (getRn0Rn == 1 || getRn0Rn == 4))
	{
		getRn0Rn++;
	}

	if (getRn0Rn == 5)
	{
		return true;
	}
	return false;
}

bool HttpClient::judgeRnRn(char nb)
{
	if (nb != '\r' && nb != '\n')
	{
		getRnRn = 0;
		return false;
	}
	if (nb == '\r' && (getRnRn == 0 || getRnRn == 2))
	{
		getRnRn++;
		return false;
	}

	if (nb == '\n' && (getRnRn == 1 || getRnRn == 3))
	{
		getRnRn++;
	}
	else
	{
		getRnRn = 0;
	}

	if (getRnRn == 4)
	{
		return true;
	}

	return false;
}

string HttpClient::Send(string method, string url, string content)
{
	HttpRequest httpRequest;
	httpRequest._method = method;
	httpRequest._httpVersion = "HTTP/1.1";
	httpRequest._requestBody = content;
	httpRequest._url = url;
	char b1[20];
	_itoa_s(content.size(), b1, 10);
	httpRequest.setAttribute("Content-Length", b1);
	httpRequest.setAttribute("Connection", "keep-alive");
	if (ip.compare("") != 0 && iPort != 0)
		httpRequest.setAttribute("Host", ip + ":" + sPort);

	unsigned int len = httpRequest.toString().size();
	const char* c1 = httpRequest.toString().c_str();
	int results = send(server, httpRequest.toString().c_str(), len, 0);
	if (results == SOCKET_ERROR)
		throw - 1;
	return httpRequest.toString();

}

string HttpClient::Send(string method, string url, map<string, string> extraAttribute, string content)
{
	HttpRequest httpRequest;
	httpRequest._method = method;
	httpRequest._httpVersion = "HTTP/1.1";
	auto key_itor = extraAttribute.begin();
	httpRequest._requestBody = content;
	char b1[20];
	_itoa_s(content.size(), b1, 10);
	httpRequest.setAttribute("Content-Length", b1);
	httpRequest.setAttribute("Connection", "keep-alive");
	if (ip.compare("") != 0 && iPort != 0)
		httpRequest.setAttribute("Host", ip + ":" + sPort);
	for (; key_itor != extraAttribute.end(); key_itor++)
	{
		string first = key_itor->first;
		string second = key_itor->second;
		httpRequest.setAttribute(first, second);
	}

	unsigned int len = httpRequest.toString().size();
	const char* c1 = httpRequest.toString().c_str();
	int results = send(server, httpRequest.toString().c_str(), len, 0);
	if (results == SOCKET_ERROR)
		throw - 1;
	return httpRequest.toString();

}

string HttpClient::ReceiveHead()
{
	current_stream_length = 0;
	unsigned char EOH = 0;     //end of head
	string l;
	//+     ： %2B
	//space ： %20
	///     ： %2F
	//?     ： %3F
	//%     ： %25
	//#     ： %23
	//&     ： %26
	//=     ： %3D
	while (EOH != 0x1)
	{
		char nb = getNextByte();
		//escape mode
		if ((nb == '\r' || nb == '\n' || nb == '\\') && enterEscapeMode == 1)
		{
			l.push_back(nb);
			enterEscapeMode = 0;
			continue;
		}

		//enter escape mode 
		if (nb == '\\')
		{
			enterEscapeMode = 1;
			continue;
		}

		//no escape mode
		//end of head
		if (judgeRnRn(nb))
		{
			EOH = 1;
			enterEscapeMode = 0;
		}
		//printf("RnRn: %ud\n", getRnRn);
		l.push_back(nb);
	}
	return l;

}

HttpClient::HttpClient()
{
	_recv_buf = new char[1024 * 1024 + 1];
	_recv_buf[1024 * 1024] = 0;
}

HttpClient::~HttpClient()
{
	delete[] _recv_buf;
}

void HttpClient::ReadBodyToFile(string filePath)
{
	//TODO
}

string HttpClient::ReadBodyToMemory()
{
	//TODO
	return string();
}

int HttpClient::StartUp(string domain, int port)
{
	//TODO 域名解析
	return 0;
}

int HttpClient::Close()
{
	return 0;
}

string HttpClient::ReadNextLineToMemory()
{
	bool escapingMode = false;
	unsigned char Rn = 0;
	string ls;
	char nextByte = getNextByte();
	while (Rn != 2)
	{
		if ((nextByte == u8'\\' || nextByte == u8'\r' || nextByte == u8'\n') && escapingMode)
		{
			Rn = 0;
			ls.push_back(nextByte);
			escapingMode = false;
			continue;
		}

		if (nextByte == u8'\\' && escapingMode == false)
		{
			Rn = 0;
			escapingMode = true;
			continue;
		}

		if (nextByte == u8'\r' && Rn == 0)
		{
			Rn++;
			continue;
		}

		if (nextByte == u8'\n' && Rn == 1)
		{
			Rn++;
			continue;
		}

		ls.push_back(nextByte);
	}
	return ls;
}

void HttpClient::ReadContentLengthToMemory(ULL len, char* buf)
{
	if (buf == NULL)
	{
		ULL it = 0;
		for (it = 0; it < len; it++)
			char nb = getNextByte();
		return;
	}

	ULL it = 0;
	for (it = 0; it < len; it++)
	{
		char nb = getNextByte();
		if( buf != NULL)
			buf[it] = nb;
	}
}

int HttpClient::StartUpIP(string ip, int port)
{
	this->ip = ip;
	this->iPort = port;
	if (port <= 0 || port >= 65536) {
		throw - 2;
		return -2;
	}
	char* portBuf = new char[7];
	errno_t err = _itoa_s(port, portBuf, 7, 10);
	if (err != 0)
	{
		//非法端口
		throw - 2;
		return -2;
	}
	sPort = portBuf;
	delete[] portBuf;

	wchar_t* wip = new wchar_t[21];
	size_t convert;
	errno_t ipRet = mbstowcs_s(&convert, wip, 21, ip.c_str(), _TRUNCATE);
	if (ipRet == -1)
	{
		//ip非法
		throw - 3;
		return -3;
	}
	
	//客户端正式启动
	//创建与服务器的Socket

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(iPort);
	InetPton(AF_INET, wip, &addr.sin_addr.S_un.S_addr);
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int connectRet = connect(server, (sockaddr*)&addr, sizeof(addr));

	if (connectRet != 0)
	{
		//SOCKET连接错误
		throw - 4;
		return -4;
	}

	delete[] wip;
	this->server = server;
	//连接成功
	string sendHeadBuf = Send("GET", u8"/tt1", "");
	send(server, sendHeadBuf.c_str(), sendHeadBuf.size(), 0);

	bool exit = false;
	while (!exit)
	{
		if (recvPackages == 1)
		{
			//有个问题，如果这个是SOCKET获取的第二个HTTP响应，
			//会和第一次响应是一样的，所以丢弃第二个响应
			string recvHead = ReceiveHead();
			HttpResponse response = HttpResponse::parseResponse(recvHead);
			SSP clh;
			response.getPair("Content-Length", &clh);
			ULL l = atoi(clh.second.c_str());
			ReadContentLengthToMemory(l, NULL);
			recvPackages++;
			continue;
		}
		string recvHead = ReceiveHead();
		HttpResponse response = HttpResponse::parseResponse(recvHead);
		//TODO 解析头部信息
		SSP clh;
		bool finded = response.getPair("Content-Length", &clh);
		if (finded)
		{
			//有Content-Length
			//测试情况下先加载到内存
			ULL l = atoi(clh.second.c_str());
			char* cs = new char[l];
			ReadContentLengthToMemory(l, cs);

			//以下是测试，具体问题还有会读取两次头部
			FILE* fp;
			fopen_s(&fp, "D:\\share\\a.txt", "wb");
			size_t st = fwrite(cs, 1, l, fp);
			fclose(fp);
			delete[] cs;
		}
		else
		{
			//无Content-Length
			SSP ckh;
			bool findChunked = response.getPair("Transfer-Encoding", &ckh);
			if (!findChunked)
			{
				//无Transfer-Encoding，不合规的头部，丢弃

			}
			else
			{
				//有Transfer-Encoding
			}
		}
		recvPackages++;
	}
	return 0;
}
