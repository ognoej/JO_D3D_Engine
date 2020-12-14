#include <Windows.h>
#include <windowsx.h>
#include "Engine.h"

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
	
	if (JEngine.Initialize(_hInst, WIN_NAME, L"MainWindow", WIN_WIDTH, WIN_HEIGHT))
	{		
		while (JEngine.ProcessMessages()==true)
		{
			JEngine.Update();
			JEngine.Render();
		}
	}

	return 0;
}
