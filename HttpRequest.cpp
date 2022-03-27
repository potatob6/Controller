#include "HttpRequest.h"
string HttpRequest::getHeaderStr(string key)
{
	auto itor = getHeaderKeypair(key);
	return itor->first + ": " + itor->second;
}

map<string, string>::iterator HttpRequest::getHeaderKeypair(string key)
{
	map<string, string>::iterator itor = _requestHead.find(key);
	return itor;
}

string HttpRequest::toString()
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

void HttpRequest::setAttribute(string key, string value)
{
	pair<string, string> p(key, value);
	_requestHead.insert(p);
}

unsigned long HttpRequest::getContentLength()
{
	return _requestBody.size();
}