#include "Engine.h"
#include <WindowsX.h>


Engine* Engine::jengine = nullptr;
Engine * Engine::getEngine()
{
	return jengine;
}
LRESULT Engine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	//case WM_ACTIVATE:
	//	if (LOWORD(wParam) == WA_INACTIVE)
	//	{
	//		mAppPaused = true;
	//		TimeMgr.Stop();
	//	}
	//	else
	//	{
	//		mAppPaused = false;
	//		TimeMgr.Start();
	//	}
	//	return 0;

		/*
		// WM_SIZE is sent when the user resizes the window.
	case WM_SIZE:
		// Save the new client area dimensions.
		mClientWidth = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
		if (md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
				{
					// If user is dragging the resize bars, we do not resize
					// the buffers here because as the user continuously
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is
					// done resizing the window and releases the resize bars, which
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;*/


		/*
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing = true;
		mTimer.Stop();
		return 0;
		*/


		/*
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing = false;
		mTimer.Start();
		OnResize();
		return 0;
		*/
		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	//case WM_MENUCHAR:
	//	// Don't beep when we alt-enter.
	//	return MAKELRESULT(0, MNC_CLOSE);
	//
	//	// Catch this message so to prevent the window from becoming too small.
	//case WM_GETMINMAXINFO:
	//	((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
	//	((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
	//	return 0;
		/*
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else if ((int)wParam == VK_F2)
			Set4xMsaaState(!m4xMsaaState);

		return 0;
	}
	*/

		
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK MessageHandler(HWND _hwnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam)
{
	return Engine::getEngine()->MsgProc(_hwnd, _uMsg, _wParam, _lParam);

	//switch (_uMsg)
	//{
	//case WM_ACTIVATE:
	//	if (LOWORD(_wParam) == WA_INACTIVE)
	//	{
	//		mAppPaused = true;
	//		TimeMgr.Stop();
	//	}
	//	else
	//	{
	//		mAppPaused = false;
	//		mTimer.Start();
	//	}
	//	return 0;
	//
	//case WM_DESTROY:
	//{
	//	PostQuitMessage(0);
	//	return 0;
	//}
	//}
	//return(DefWindowProc(_hwnd, _uMsg, _wParam, _lParam));
}

Engine::Engine()
{
   assert(Engine::jengine == nullptr);
   Engine::jengine = this;
}

bool Engine::Initialize(HINSTANCE _hInstance, LPCWSTR  _window_title, std::wstring _window_class, int _width, int _height)
{


	//TimeMgr.Start();

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
	//wndClass.lpfnWndProc = MessageHandler;
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



	if (InitDx(handle, _width, _height) == false)
	{
		MessageBox(nullptr, L"초기화 실패", L"오류", MB_OK);
		return 0;
	}

	//int c = sizeof(DxMgr.MyObjectMgr.GetObjects("jigu")->model->meshes);
	//int b = sizeof(DxMgr.MyObjectMgr.GetObjects("jigu")->model->meshes[0].bones);

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

void Engine::Update(MyTimer& TimeMgr)
{
	//float dt = TimeMgr.GetMilisecondsElapsed();
	//TimeMgr.Restart();

	DxMgr.Update(TimeMgr);


	//DxMgr.MyObjectMgr.GetObjects("jigu")->AdjustRotation(0.0f, 0.._1101f*dt, 0.0f);
	//DxMgr.MyObjectMgr.findobj->AdjustRotation(0.0f, 0.1f, 0.0f);
}

void Engine::Render(MyTimer& TimeMgr)
{
	DxMgr.Render(TimeMgr);
}

bool Engine::InitDx(HWND _hWnd,int _width,int _height)
{
	if (!DxMgr.Initialize(_hWnd, _width, _height))
	{
		return false;
	}

	//DxMgr.NewObject("Sphere", "Sphere");
	
	return true;
}

int Engine::Run()
{
	MSG msg = { 0 };

	TimeMgr.Reset();

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{

			if (!mAppPaused)
			{

				TimeMgr.Tick();
				//CalculateFrameStats();
				Update(TimeMgr);
				Render(TimeMgr);
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;



}
