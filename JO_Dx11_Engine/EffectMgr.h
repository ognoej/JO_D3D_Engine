#pragma once
#include <d3d11.h>
#include <Windows.h>
#define SafeDelete(x) { delete x; x = 0; }
#include <string>
#include "d3d11effect.h"
#include "Utilheaders.h"

class EffectMgr
{
public:
	EffectMgr(ID3D11Device* device, const std::wstring& filename);
	virtual ~EffectMgr();

private:
	EffectMgr(const EffectMgr& rhs);
	EffectMgr& operator=(const EffectMgr& rhs);

protected:
	ID3DX11Effect* mFX;

};

