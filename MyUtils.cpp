#include "MyUtils.h"
void MyUtils::pushStringToList(string n, list<char>& chars)
{
	auto itor = n.begin();
	for (; itor != n.end(); itor++)
	{
		chars.push_back(*itor);
	}
}