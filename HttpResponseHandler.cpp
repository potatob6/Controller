#include "HttpResponseHandler.h"
#include "HttpClient.h"
#include "HttpResponse.h"

HttpResponseHandler::HttpResponseHandler(HttpClient* client, HttpResponse* response)
{
	this->client = client;
	this->response = response;
}

//能进入此的一般都是文件类型，所以暂时先不用区分类型
errno_t HttpResponseHandler::Handle()
{
	SSP clh, rspm, connm;
	SSP trf;
	bool finded = response->getPair("Content-Length", &clh);
	bool findedTranser = response->getPair("Transfer-Encoding", &trf);
	//bool cmdm = response->getPair("Response-Mode", &rspm);
	//bool conn = response->getPair("Connection", &connm);
	if (finded)
	{
		//有Content-Length
		//测试情况下先加载到内存
		ULL l = atoi(clh.second.c_str());
		char* cs = new char[l + 1];
		cs[l] = 0;
		try {
			client->ReadContentLengthToMemory(l, cs);

		}
		catch (int e) {
			delete[] cs;
			//接收失败
			//将当前MessageNo以及结果返回给WebSocket
			//TODO
			return e;
		}

		delete[] cs;
		return 0;
	}
	else if(findedTranser)
	{

		return 0;
	}
	return 1;
}