#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <string>
#include "Graphics\MyD3D_Mgr.h"
#include "MyTimer.h"


class Engine
{
	//MyD3D_Mgr DxMgr;

public:

	// 윈도우 핸들러
	HWND handle = NULL; 
	HINSTANCE hInstance = NULL;

	// DX매니져
	MyD3D_Mgr DxMgr;
	MyTimer TimeMgr;

	// Window 크기
	int width = 0;
	int height = 0;

	Object m_earth;

	//윈도우 이름
	std::wstring window_class_wide = L"";

	bool	Initialize(HINSTANCE _hInstance, LPCWSTR _window_title, std::wstring _window_class, int _width, int _height);
	bool	ProcessMessages();
	void	Update(float _delayTime = 0);
	void	Render(float _delayTime = 0);
	bool	InitDx(HWND _hWnd, int _width, int _height);

	//XMMATRIX  viewProjectionMatrix;
};

