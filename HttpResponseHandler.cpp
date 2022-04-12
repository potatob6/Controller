#include "HttpResponseHandler.h"
#include "HttpClient.h"
#include "HttpResponse.h"

HttpResponseHandler::HttpResponseHandler(HttpClient* client, HttpResponse* response)
{
	this->client = client;
	this->response = response;
}

errno_t HttpResponseHandler::Handle()
{
	SSP clh, rspm, connm;
	bool finded = response->getPair("Content-Length", &clh);
	bool cmdm = response->getPair("Response-Mode", &rspm);
	bool conn = response->getPair("Connection", &connm);
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
			return e;
		}
		//TODO 做完返回响应的模块处理
		cout << u8"来自服务器的来信" << endl;
		cout << cs << endl;
		if (cmdm && rspm.second.compare("command") == 0) {
			cout << u8"执行命令:" << cs << endl;
			//system(cs);
		}
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
		bool findChunked = response->getPair("Transfer-Encoding", &ckh);
		if (!findChunked)
		{
			//无Transfer-Encoding，不合规的头部，丢弃

		}
		else
		{
			//有Transfer-Encoding
		}
	}

	return 0;
}