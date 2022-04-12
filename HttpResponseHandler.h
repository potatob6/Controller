#pragma once
#include <corecrt.h>
class HttpClient;
class HttpResponse;

class HttpResponseHandler
{
private:
	HttpClient* client;
	HttpResponse* response;
public:
	HttpResponseHandler(HttpClient* client, HttpResponse* response);
	errno_t Handle();
};

