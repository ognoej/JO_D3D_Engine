#pragma once
#include "MyString.h"
#include <comdef.h>
#include <Windows.h>
#include <string>

#define COM_ERROR_IF_FAILED( hr, msg ) if( FAILED( hr ) ) throw Log( hr, msg )

class MyErrorCheck
{
public:

	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void Log(HRESULT hr, std::wstring message);

};

