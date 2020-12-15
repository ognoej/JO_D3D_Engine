#pragma once
#include "MyString.h"
#include <comdef.h>
#include <Windows.h>
#include <string>

class MyErrorCheck
{
public:

	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void Log(HRESULT hr, std::wstring message);

};

