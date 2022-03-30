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
		regex attris("(.+)[ ]*:[ ]*(.+)\r\n");
		smatch result_attris;

		//legel http response head
		//TODO only one attribute can be matched
		for (int i = 0; i < result.size(); i++)
		{
			cout << u8"匹配结果" << endl;
			cout << "result[" << i << "]: " << result[i] << endl;
		}
		return HttpResponse();
	}
	else
	{
		cout << u8"非法Http响应" << endl;
	}
	throw - 1;
	return HttpResponse();
}