#pragma once
#include "MyString.h"
#include <comdef.h>
#include <Windows.h>
#include <string>

#define COM_ERROR_IF_FAILED( hr, msg ) if( FAILED( hr ) ) throw MyErrorCheck( hr, msg )

class MyErrorCheck
{
public:
	MyErrorCheck(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
	{
		_com_error error(hr);
		whatmsg = L"Msg: " + MyString::toWstring(std::string(msg)) + L"\n";
		whatmsg += error.ErrorMessage();
		whatmsg += L"\nFile: " + MyString::toWstring(file);
		whatmsg += L"\nFunction: " + MyString::toWstring(function);
		whatmsg += L"\nLine: " + MyString::toWstring(std::to_string(line));
	}

public:

	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void Log(HRESULT hr, std::wstring message);

private:
	std::wstring whatmsg;

};

