#include "HttpResponse.h"

using namespace std;
HttpResponse HttpResponse::parseResponse(string resp)
{
	char* buf = new char[10];
	regex re("(HTTP/1.[10]) ([0-9]{3}).*\r\n((.+)[ ]*:[ ]*(.+)\r\n)*\r\n");
	//regex re("(HTTP/1.[01]) ([0-9]{3})([ ]*.*)\n((.+)[ ]*:[ ]*(.+)\n)*");
	smatch result;
	bool match_re = regex_search(resp, result, re);
	if (match_re)
	{
		//split the string

		list<string> *lparameters = new list<string>;
		const char* delimiter = "\r\n";
		char* str = (char*)resp.c_str();
		char* token = NULL;
		char* next_token = NULL;

		token = strtok_s(str, delimiter, &next_token);

		while (token != NULL)
		{
			if (strcmp(token, "") != 0)
			{
				lparameters->push_back(token);
			}
			token = strtok_s(NULL, delimiter, &next_token);
		}

		regex attris("(.+)[ ]*:[ ]*(.+)");     //HTTP属性匹配表达式

		//TODO 遍历获取每个HTTP属性
		auto itor = lparameters->begin();
		for (; itor != lparameters->end(); itor++)
		{
			smatch result_attris;
			regex_search(*itor, result_attris, attris);
			printf_s("Key: %s, Value: %s\n", result_attris[1], result_attris[2]);
		}
		delete lparameters;
		return HttpResponse();
	}
	else
	{
		cout << u8"非法Http响应" << endl;
	}
	throw - 1;
	return HttpResponse();
}