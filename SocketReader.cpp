
#include "SocketReader.h"
SocketReader::SocketReader(SOCKET socket)
{
	this->server = socket;
	this->recv_buf = new char[4096];
}

SocketReader::~SocketReader()
{
	delete[] this->recv_buf;
}