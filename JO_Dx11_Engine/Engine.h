#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <string>
#include "Graphics\MyD3D_Mgr.h"
#include "..\Utilites\MyTimer.h"


class Engine
{
	//MyD3D_Mgr DxMgr;
public:
	Engine();

	// ������ �ڵ鷯
	HWND handle = NULL; 
	HINSTANCE hInstance = NULL;

	// DX�Ŵ���
	MyD3D_Mgr DxMgr;
	MyTimer TimeMgr;

	// Window ũ��
	int width = 0;
	int height = 0;

	Object m_earth;

	//������ �̸�
	std::wstring window_class_wide = L"";
	bool      mAppPaused = false;

	bool	Initialize(HINSTANCE _hInstance, LPCWSTR _window_title, std::wstring _window_class, int _width, int _height);
	bool	ProcessMessages();
	void	Update(MyTimer& TimeMgr);
	void	Render(MyTimer& TimeMgr);
	bool	InitDx(HWND _hWnd, int _width, int _height);
	int		Run();

public:
	static Engine* jengine;
	static Engine* getEngine();
	 LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//XMMATRIX  viewProjectionMatrix;
};

