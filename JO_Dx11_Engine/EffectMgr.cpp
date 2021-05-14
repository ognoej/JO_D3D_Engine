#include "EffectMgr.h"

// fx파일 읽어오기
Effect::Effect(ID3D11Device * device, const std::wstring & filename)
	: mFX(0)
{
	  std::ifstream fin(filename.c_str(), std::ios::binary);
	  
	  fin.seekg(0, std::ios_base::end);
	  int size = (int)fin.tellg();
	  fin.seekg(0, std::ios_base::beg);
	  std::vector<char> compiledShader(size);
	  
	  fin.read(&compiledShader[0], size);
	  fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, device, &mFX));
}

void Effects::InitAll(ID3D11Device * device)
{

	// Skinnedmesh용 노말맵 쉐이더 (컴파일 완료)
	SsaoNormalDepthFX = new SsaoNormalDepthEffect(device, L"FX/SsaoNormalDepth.fxo");

	/*
	BasicFX = new BasicEffect(device, L"FX/Basic.fxo");
	NormalMapFX = new NormalMapEffect(device, L"FX/NormalMap.fxo");
	BuildShadowMapFX = new BuildShadowMapEffect(device, L"FX/BuildShadowMap.fxo");
	SsaoFX = new SsaoEffect(device, L"FX/Ssao.fxo");
	SsaoBlurFX = new SsaoBlurEffect(device, L"FX/SsaoBlur.fxo");
	SkyFX = new SkyEffect(device, L"FX/Sky.fxo");
	DebugTexFX = new DebugTexEffect(device, L"FX/DebugTexture.fxo");
	*/
}
