#include <Windows.h>
#include <windowsx.h>
#include "Engine.h"
#include "MyTimer.h"


#define	WIN_NAME	L"JO_Engine"
#define WIN_WIDTH	800
#define	WIN_HEIGHT	450


int APIENTRY WinMain(
	HINSTANCE	_hInst,
	HINSTANCE	_hPrevInst,
	LPSTR		_cmdParam,
	int			_cmdShow)
{
	Engine JEngine;

	if (!JEngine.Initialize(_hInst, WIN_NAME, L"MainWindow", WIN_WIDTH, WIN_HEIGHT))
	{
		return 0;
	}
	else
	{
		JEngine.Run();
	}

	////try
	////{
	//	Engine JEngine;
	//	//JEngine.jengine = &JEngine;
	//	if (!JEngine.Initialize(_hInst, WIN_NAME, L"MainWindow", WIN_WIDTH, WIN_HEIGHT))
	//		return 0;
	//
	// JEngine.Run();
	////}
	//catch
	//{
	//
	//}
	//catch (DxException& e)
	//{
	//	MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
	//	return 0;
	//}

	// Engine JEngine;
	// bool      mAppPaused = false; 
	// 
	// if (!JEngine.Initialize(_hInst, WIN_NAME, L"MainWindow", WIN_WIDTH, WIN_HEIGHT))
	// {
	// 	return 0;
	// 
	// }
	// else
	// {
	// 	JEngine.Run();
	// }

		//while (msg.message != WM_QUIT)
		//{
		//	// If there are Window messages then process them.
		//	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		//	{
		//		TranslateMessage(&msg);
		//		DispatchMessage(&msg);
		//	}
		//	// Otherwise, do animation/game stuff.
		//	else
		//	{
		//		TimeMgr.Tick();
		//
		//		if (!mAppPaused)
		//		{
		//			TimeMgr.Tick();
		//			JEngine.Update(TimeMgr);
		//			JEngine.Render(TimeMgr);
		//		}
		//		else
		//		{
		//			Sleep(100);
		//		}
		//	}
		//}


	return 0;
}
