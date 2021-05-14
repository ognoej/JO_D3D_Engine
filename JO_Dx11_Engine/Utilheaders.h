#pragma once

#include <d3d11.h>
#include "d3d11effect.h"
#include <DirectXMath.h>
//#include <dxerr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "MathHelper.h"
//#include "LightHelper.h"
//using namespace DirectX;



HRESULT WINAPI DXTraceA(__in_z const char* strFile, __in DWORD dwLine, __in HRESULT hr, __in_z_opt const char* strMsg, __in BOOL bPopMsgBox);
HRESULT WINAPI DXTraceW(__in_z const char* strFile, __in DWORD dwLine, __in HRESULT hr, __in_z_opt const WCHAR* strMsg, __in BOOL bPopMsgBox);


#ifdef UNICODE
#define DXTrace DXTraceW
#else
#define DXTrace DXTraceA
#endif 


//
// Helper macros
//
#if defined(DEBUG) | defined(_DEBUG)
#define DXTRACE_MSG(str)              DXTrace( __FILE__, (DWORD)__LINE__, 0, str, FALSE )
#define DXTRACE_ERR(str,hr)           DXTrace( __FILE__, (DWORD)__LINE__, hr, str, FALSE )
#define DXTRACE_ERR_MSGBOX(str,hr)    DXTrace( __FILE__, (DWORD)__LINE__, hr, str, TRUE )
#else
#define DXTRACE_MSG(str)              (0L)
#define DXTRACE_ERR(str,hr)           (hr)
#define DXTRACE_ERR_MSGBOX(str,hr)    (hr)
#endif




#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif