#include "Engine.h"


LRESULT CALLBACK MessageHandler(HWND _hwnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return(DefWindowProc(_hwnd, _uMsg, _wParam, _lParam));
}


bool Engine::Initialize(HINSTANCE _hInstance, LPCWSTR  _window_title, std::wstring _window_class, int _width, int _height)
{
	// 윈도우 초기화
	this->hInstance = _hInstance;
	this->width = _width;
	this->height = _height;
	this->window_class_wide = _window_class; //wide string representation of class string (used for registering class and creating window)


	RECT rt = { 0, 0, _width , _height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

	auto width = rt.right - rt.left;
	auto height = rt.bottom - rt.top;

	int a;



	// 윈도우 등록
	WNDCLASSEX	wndClass = {};
	//wndClass.lpfnWndProc = WndProc;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Fl
	wndClass.lpfnWndProc = MessageHandler; //Pointer to W
	wndClass.cbClsExtra = 0; //# of extra bytes to allocate f
	wndClass.cbWndExtra = 0; //# of extra bytes to allocate f
	wndClass.hInstance = _hInstance; //Handle to the ins
	wndClass.hIcon = NULL;   //Handle to the class icon. Must
	wndClass.hIconSm = NULL; //Handle to small icon for this 
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //Default
	wndClass.hbrBackground = NULL; //Handle to the class back
	wndClass.lpszMenuName = NULL; //Pointer to a null termina
	wndClass.lpszClassName = _window_title;
	wndClass.cbSize = sizeof(WNDCLASSEX); //Need to fill in t
	RegisterClassEx(&wndClass);
	

	// 윈도우 핸들 생성
	this->handle = CreateWindow(
		wndClass.lpszClassName,
		wndClass.lpszClassName,
		WS_OVERLAPPEDWINDOW,
		0, 0, width, height,
		nullptr, nullptr, nullptr, nullptr);

	if (handle == nullptr)
	{
		MessageBox(nullptr, L"윈도우생성 실패", L"오류", MB_OK);
		return 0;
	}

	ShowWindow(handle, SW_SHOW);


	
	if (InitDx(handle,_width,_height) == false)
	{
		MessageBox(nullptr, L"초기화 실패", L"오류", MB_OK);
		return 0;
	}

	return true;
}

bool Engine::ProcessMessages()
{
	//메세지 담기용
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

	while (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
		this->handle, //Handle to window we are checking messages for
		0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
		0,    //Maximum Filter Msg Value
		PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
	{
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
	}

	// 윈도우 종료 메세지
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL; //Message processing loop takes care of destroying this window
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}
	return true;
}

void Engine::Update(float _delayTime)
{
}

void Engine::Render()
{
	DxMgr.Render(viewProjectionMatrix);
}

bool Engine::InitDx(HWND _hWnd,int _width,int _height)
{
	if (!DxMgr.Initialize(_hWnd, _width, _height))
	{
		return false;
	}
	return true;
}
