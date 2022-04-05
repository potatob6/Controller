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
		if (total_recv == 0)
		{
			//socket已经断开
			//重试三次
			for (int i = 0; i < RETRY_TIMES; i++)
			{
#ifdef DEBUG_MODE
				cout << u8"Recv失败，正在重试第" << i+1 << u8"次" << endl;
#endif
				try{
					getConnection();
					throw - 7;      //抛出-7异常让StartUp重新启动
				}
				catch (int e)
				{
					if (e == -7)
						throw - 7;
					Sleep(HTTPCLIENT_RETRY_DELAY);
					continue;
				}
			}
			throw - 1;
		}
		if (total_recv < 0)
		{
#ifdef DEBUG_MODE
			int RET = WSAGetLastError();
			cout << u8"total_recv返回码" << total_recv << endl;
			cout << u8"WSAGetLastError返回码" << RET << endl;
#endif
			//socket已经断开
			//重试三次
			for (int i = 0; i < RETRY_TIMES; i++)
			{
#ifdef DEBUG_MODE
				cout << u8"Recv失败，正在重试第" << i + 1 << u8"次" << endl;
#endif
				try {
					getConnection();
					throw - 7;      //抛出-7异常让StartUp重新启动
				}
				catch (int e)
				{
					if (e == -7)
						throw - 7;
					Sleep(HTTPCLIENT_RETRY_DELAY);
					continue;
				}
			}
			throw - 5;
		}

		if(total_recv>=0)
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
resend:
	LL results = send(server, httpRequest.toString().c_str(), len, 0);
#ifdef DEBUG_MODE
	cout << u8"发送报文" << httpRequest.toString() << endl;
#endif
	if (results == SOCKET_ERROR)
	{
#ifdef DEBUG_MODE
		cout << u8"Send失败，尝试" << RETRY_TIMES << u8"次建立新连接发送" << endl;
#endif

		//尝试三次建立新的TCP连接
		for (int i = 0; i < RETRY_TIMES; i++)
		{
			try {
#ifdef DEBUG_MODE
				cout << u8"正在尝试第" << i + 1 << u8"次连接" << endl;
#endif
				getConnection();
				cout << u8"重连成功" << endl;
				goto resend;
				break;
			}
			catch (int e)
			{
				Sleep(HTTPCLIENT_RETRY_DELAY);
				continue;
			}
		}
		throw - 1;
	}
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
resend:
	int results = send(server, httpRequest.toString().c_str(), len, 0);
	if (results == SOCKET_ERROR)
	{
#ifdef DEBUG_MODE
		cout << u8"Send失败，尝试" << RETRY_TIMES << u8"次建立新连接发送" << endl;
#endif

		//尝试三次建立新的TCP连接
		for (int i = 0; i < RETRY_TIMES; i++)
		{
			try {
#ifdef DEBUG_MODE
				cout << u8"正在尝试第" << i + 1 << u8"次连接" << endl;
#endif
				getConnection();
				ResetAllFlags();
				cout << u8"重连成功" << endl;
				goto resend;
				break;
			}
			catch (int e)
			{
				Sleep(HTTPCLIENT_RETRY_DELAY);
				continue;
			}
		}
		throw - 1;
	}
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
	_recv_buf = NULL;
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

void HttpClient::getConnection()
{
	if (server == 0)
		closesocket(server);
	wchar_t* wip = new wchar_t[21];
	size_t convert;
	errno_t ipRet = mbstowcs_s(&convert, wip, 21, ip.c_str(), _TRUNCATE);
	if (ipRet == -1)
	{
		//ip非法
		delete[] wip;
		throw - 3;
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
	if (server == INVALID_SOCKET)
	{
		delete[] wip;
		throw - 8;
	}
	int connectRet = connect(server, (sockaddr*)&addr, sizeof(addr));

	if (connectRet != 0)
	{
		//SOCKET连接错误
		delete[] wip;
		throw - 4;
	}

	delete[] wip;
	this->server = server;
	ResetAllFlags();
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

void HttpClient::ResetAllFlags()
{
	byte_pointer = 0;
	total_recv = 0;
	current_stream_length = 0;
	getRnRn = 0;
	getRn0Rn = 0;
	enterEscapeMode = 0;
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
		delete[] portBuf;
		throw - 2;
		return -2;
	}
	sPort = portBuf;
	delete[] portBuf;

	try {
		getConnection();
	}
	catch (int e)
	{
		throw e;
		return e;
	}
	//连接成功

	bool exit = false;
	while (!exit)
	{
		string sendHeadBuf = Send("GET", u8"/tt1", "");

		try {
			string recvHead = ReceiveHead();
			HttpResponse response = HttpResponse::parseResponse(recvHead);
			//TODO 解析头部信息
			SSP clh, rspm, connm;
			bool finded = response.getPair("Content-Length", &clh);
			bool cmdm = response.getPair("Response-Mode", &rspm);
			bool conn = response.getPair("Connection", &connm);
			if (finded)
			{
				//有Content-Length
				//测试情况下先加载到内存
				ULL l = atoi(clh.second.c_str());
				char* cs = new char[l + 1];
				cs[l] = 0;
				try {
					ReadContentLengthToMemory(l, cs);
				}
				catch (int e) {
					delete[] cs;
					throw e;
				}
				cout << u8"来自服务器的来信" << endl;
				if (cmdm && rspm.second.compare("command") == 0) {
					cout << u8"执行命令:" << cs << endl;
					//system(cs);
				}
				//以下是测试，具体问题还有会读取两次头部
				//FILE* fp;
				//fopen_s(&fp, "D:\\share\\a.txt", "wb");
				//size_t st = fwrite(cs, 1, l, fp);
				//fclose(fp);
				if (cmdm && rspm.second.compare("messgage-box") == 0)
				{
					cout << u8"执行消息盒子" << endl;
					wchar_t* wcs = new wchar_t[l + 1];
					mbstowcs_s((size_t*)&l, wcs, l, cs, (size_t)l);
					delete[] wcs;
				}
				delete[] cs;
				Sleep(50);
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
			recvResponses++;
			//if (conn && connm.second.compare("close") == 0)
			//{
			//	cout << u8"Http报文返回服务器主动断开连接" << endl;
			//	break;
			//}
		}
		catch (int e)
		{
			//重新连接的信号以及非法头部的信号
			if (e == -7)
			{
				//重置各种信号
				ResetAllFlags();
				continue;
			}
			if (e == -8)
			{
				continue;
			}
			else
			{
				throw e;
			}
		}
	}
	return 0;
}
