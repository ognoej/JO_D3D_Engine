#include "MyErrorCheck.h"

void MyErrorCheck::Log(std::string message)
{
	std::string error_message = "���� �߻�: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}


void MyErrorCheck::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring error_message = L"���� �߻� : " + MyString::toWstring(message) + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}



void MyErrorCheck::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring error_message = L"���� �߻�: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
