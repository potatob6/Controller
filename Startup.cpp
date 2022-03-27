#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <map>
#include <string>
#include <regex>
#include <list>
#include <iostream>
#include "SocketReader.h"
#pragma comment(lib, "ws2_32")

using namespace std;
class HttpRequest {
public:
	string _method;
	string _url;
	string _httpVersion;
	map<string, string> _requestHead;
	string _requestBody;
	
	string getHeaderStr(string key) 
	{
		auto itor = getHeaderKeypair(key);
		return itor->first + ": " + itor->second;
	}
	map<string, string>::iterator getHeaderKeypair(string key) 
	{
		map<string, string>::iterator itor = _requestHead.find(key);
		return itor;
	}
	string toString() 
	{
		string str1;
		str1 += this->_method + " ";         //method
		str1 += this->_url + " ";            //url
		str1 += this->_httpVersion + "\r\n"; //http version
		map<string, string>::iterator itor;
		for (itor = _requestHead.begin(); itor != _requestHead.end(); itor++) 
		{
			str1 += itor->first + ": " + itor->second + "\r\n";
		}
		str1 += "\r\n";
		str1 += _requestBody;
		return str1;
	}
	void setAttribute(string key, string value) 
	{
		pair<string, string> p(key, value);
		_requestHead.insert(p);
	}
	unsigned long getContentLength() 
	{
		return _requestBody.size();
	}
};

class HttpClient {
private:
	char* _recv_buf;
	unsigned long byte_pointer = 0;
	unsigned long total_recv = 0;
	unsigned long current_stream_length = 0;
	unsigned char getRnRn = 0;            // end of head bytes \r \n \r \n
	unsigned char getRn0Rn = 0;           // end of chuncked   \r \n 0 \r \n
	unsigned char enterEscapeMode = 0;
	char getNextByte()
	{
		if (byte_pointer == total_recv)
		{
			//read from socket
			total_recv = recv(server, _recv_buf, 1024 * 1024, 0);
			if (total_recv <= 0)
			{
				throw -1;
			}
			_recv_buf[total_recv] = 0;
			byte_pointer = 0;
		}
		char _temp = _recv_buf[byte_pointer];
		byte_pointer++;
		current_stream_length++;
		return _temp;
	}

	//read next byte and judge is this stream read end of http response
	bool judgeRn0Rn(char nb)
	{
		if (current_stream_length == 10)
		{
			printf("1234\n");
		}
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

	//read next byte and judge is this stream read end of http head
	bool judgeRnRn(char nb)
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
public:
	SOCKET server;
	string ip;
	string port;


	string Send(string method, string url, string content)
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
		if (ip.compare("") != 0 && port.compare("") != 0)
			httpRequest.setAttribute("Host", ip + ":" + port);

		unsigned int len = httpRequest.toString().size();
		const char* c1 = httpRequest.toString().c_str();
		int results = send(server, httpRequest.toString().c_str(), len, 0);
		if (results == SOCKET_ERROR)
			throw - 1;
		return httpRequest.toString();
	}
	string Send(string method, string url, map<string, string> extraAttribute, string content)
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
		if (ip.compare("") != 0 && port.compare("") != 0)
			httpRequest.setAttribute("Host", ip + ":" + port);
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
	string ReceiveHead()
	{
		unsigned char EOH = 0;     //end of head
		list<char> l;
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
			}
			//printf("RnRn: %ud\n", getRnRn);
			l.push_back(nb);
		}
		auto itor = l.begin();
		char* rb = new char[l.size()+1];
		rb[l.size()] = 0;
		int i = 0;
		for (; itor != l.end(); itor++)
		{
			rb[i] = (*itor);
			i++;
		}
		string n(rb);
		delete[] rb;
		return string(n);
	}
	HttpClient(SOCKET socket)
	{
		server = socket;
		_recv_buf = new char[1024 * 1024 + 1];
		_recv_buf[1024 * 1024] = 0;
	}
	~HttpClient()
	{
		delete[] _recv_buf;
	}
};

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
		cout << u8"本地发送的消息" << endl;
		cout << sendStr;
		cout << u8"end" << endl;
	}
	catch (int e)
	{
		cout << u8"发送失败" << endl;
	}

	try 
	{
		string resp = httpClient.ReceiveHead();
		cout << u8"服务器" << endl;
		cout << resp;
	}
	catch (int e)
	{
		cout << u8"失败" << endl;
	}

	closesocket(server);

	WSACleanup();

	return 0;
}
