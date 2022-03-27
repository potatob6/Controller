#include "HttpClient.h"
#include "HttpRequest.h"

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
	if (ip.compare("") != 0 && port.compare("") != 0)
		httpRequest.setAttribute("Host", ip + ":" + port);

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

string HttpClient::ReceiveHead()
{
	unsigned char EOH = 0;     //end of head
	list<char> l;
	//+     £º %2B
	//space £º %20
	///     £º %2F
	//?     £º %3F
	//%     £º %25
	//#     £º %23
	//&     £º %26
	//=     £º %3D
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
	char* rb = new char[l.size() + 1];
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

HttpClient::HttpClient(SOCKET socket)
{
	server = socket;
	_recv_buf = new char[1024 * 1024 + 1];
	_recv_buf[1024 * 1024] = 0;
}

HttpClient::~HttpClient()
{
	delete[] _recv_buf;
}