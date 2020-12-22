#pragma once
#include <string>
#include <algorithm>

class MyString
{
public:
	static std::wstring toWstring(std::string _str);
	static std::string GetDirectoryFromPath(const std::string & filepath);
	static std::string GetFileExtension(const std::string & filename);
};

