#include "MyString.h"

std::wstring MyString::toWstring(std::string _str)
{
	std::wstring wide_string(_str.begin(), _str.end());
	return wide_string;
}
