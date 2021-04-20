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

	MyTimer TimeMgr;
	Engine JEngine;
	
	TimeMgr.Start();

	if (JEngine.Initialize(_hInst, WIN_NAME, L"MainWindow", WIN_WIDTH, WIN_HEIGHT))
	{	

		while (JEngine.ProcessMessages()==true)
		{
			TimeMgr.Tick();
			JEngine.Update(TimeMgr);
			JEngine.Render(TimeMgr);
		}
	}

	return 0;
}
