#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include "shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "ObjectMgr.h"
#include "..\Utilites\MyCamera.h"



using namespace Microsoft::WRL;

class MyD3D_Mgr
{
private:
	MyCamera cameraMgr;


public:

	~MyD3D_Mgr();
	bool Initialize(HWND hwnd, int width, int height);
	void Render();

	bool NewObject(std::string objname, std::string filepath);

	
	int windowWidth = 0;
	int windowHeight = 0;

	ObjectMgr MyObjectMgr;
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;


	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState_CullFront;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;

	MyConstBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	MyConstBuffer<CB_PS_pixelshader> cb_ps_pixelshader;

	//Ω¶¿Ã¥ı
	VertexShader	vertexshader;
	PixelShader		pixelshader;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;
};
