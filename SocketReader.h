#include <WinSock2.h>
#include <string>
using namespace std;
class SocketReader
{
public:
	SOCKET server;
	char* recv_buf;
	void(*headerHandler);
	
	SocketReader(SOCKET socket);
	~SocketReader();
};
