#include "EffectMgr.h"

// fx파일 읽어오기
EffectMgr::EffectMgr(ID3D11Device * device, const std::wstring & filename)
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
