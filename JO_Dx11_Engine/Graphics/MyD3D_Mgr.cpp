#include "MyD3D_Mgr.h"
#include <vector>


MyD3D_Mgr::~MyD3D_Mgr()
{

}

bool MyD3D_Mgr::Initialize(HWND hwnd, int width, int height)
{

		this->windowWidth = width;
		this->windowHeight = height;

#pragma region DX 초기화작업

		//	스왑체인용
		DXGI_SWAP_CHAIN_DESC scd = { 0 };

		scd.BufferDesc.Width					=	this->windowWidth;
		scd.BufferDesc.Height					=	this->windowHeight;
		scd.BufferDesc.Format					=	DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Scaling					=	DXGI_MODE_SCALING_UNSPECIFIED;
		scd.SwapEffect							=	DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags								=	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.SampleDesc.Count					=	1;
		scd.SampleDesc.Quality					=	0;
		scd.BufferUsage							=	DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount							=	1;
		scd.OutputWindow						=	hwnd;
		scd.Windowed							=	TRUE;
		scd.BufferDesc.ScanlineOrdering			=	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.RefreshRate.Numerator	=	60;
		scd.BufferDesc.RefreshRate.Denominator	=	1;

		// dx 버전 호환용
		D3D_FEATURE_LEVEL	featureArray[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
		};

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,								//	모니터 어댑터
			D3D_DRIVER_TYPE_HARDWARE,				//	하드웨어가속(T&L) 관련
			nullptr,								//	
			D3D11_CREATE_DEVICE_SINGLETHREADED,		//	스레드 옵션
			featureArray,//featureArray,			//	지원가능 dx버전
			ARRAYSIZE(featureArray),				//	배열의 개수
			D3D11_SDK_VERSION,						//	SDK 버전
			&scd,									//	스왑체인 구조체
			this->swapchain.GetAddressOf(),			//	스왑체인
			this->device.GetAddressOf(),			//	장치
			nullptr,								//	결정된 dx레벨 저장 변수
			this->deviceContext.GetAddressOf());	//	문맥장치

		if (FAILED(hr))
		{
			MessageBoxA(nullptr, "Failed to create device and swapchain.", "FAILED", MB_OK);
			return false;
		}

		//	랜더타켓용 버퍼 얻기
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		FAILED(hr, "GetBuffer Failed.");

		//	랜더타켓 생성
		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		FAILED(hr, "Failed to create render target view.");
		backBuffer.ReleaseAndGetAddressOf(); ////////////수정한사항

		// 스탠실 생성
		CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->windowWidth, this->windowHeight);
		depthStencilTextureDesc.MipLevels = 1;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hr = this->device->CreateTexture2D(&depthStencilTextureDesc, NULL, this->depthStencilBuffer.GetAddressOf());
		FAILED(hr, "Failed to create depth stencil buffer.");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
		FAILED(hr, "Failed to create depth stencil view.");

		CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
		depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
		FAILED(hr, "Failed to create depth stencil state.");

		// 랜더타겟 등록
		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		// 뷰포트 설정
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->windowWidth), static_cast<float>(this->windowHeight));
		this->deviceContext->RSSetViewports(1, &viewport);

		// 레스터라이즈 설정
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
		FAILED(hr, "Failed to create rasterizer state.");

		// 프론트 컬링설정
		CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
		rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
		FAILED(hr, "Failed to create rasterizer state.");

		// 블렌더 설정
		D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC blendDesc = { 0 };
		blendDesc.RenderTarget[0] = rtbd;

		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		FAILED(hr, "Failed to create blend state.");

		// 스트라이프 & 폰트(UI 애니메이션용)
		//spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());  
		//spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		// 샘플러
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
		FAILED(hr, "Failed to create sampler state.");


		// 쉐이더 레이아웃 설정
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
			{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		};

		UINT numElements = ARRAYSIZE(layout);



		//쉐이더 등록
		std::wstring shaderfolder = L"";


			//버텍스
		if (!vertexshader.Initialize(this->device, shaderfolder+  L"vertexshader.cso", layout, numElements))
			return false;
			//픽셀
		if (!pixelshader.Initialize(this->device, shaderfolder+ L"pixelshader.cso"))
			return false;


		// 상수버퍼 초기화
		hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

#pragma endregion



#pragma region 카메라 기본설정

		cameraMgr.SetPosition(0.0f, 100.0f, -150.0f);
		cameraMgr.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.0f);

#pragma endregion



#pragma region OBJ관리

		MyObjectMgr.initialize();

		//NewObject("jigu", "Data\\earth.obj");

		//NewObject("jigu", "Data\\rp_manuel_animated_001_dancing.fbx");
		NewObject("jigu", "Data\\rp_alison_rigged_001_CAT_vray_CAMERA.fbx");

#pragma endregion
	return true;

}

void MyD3D_Mgr::Render()
{	

	if (deviceContext == nullptr) return;
	
	float bgcolor[] = { 0.0f,0.0f, 0.0f, 0.0f };
	//	버퍼 청소
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

	this->MyObjectMgr.DrawObjects(cameraMgr.GetViewMatrix()*cameraMgr.GetProjectionMatrix());

	//this->deviceContext->Draw(0,0);
	this->swapchain->Present(0, NULL);

	

}

bool MyD3D_Mgr::NewObject(std::string objname, std::string filepath)
{

	 MyObjectMgr.AddObject(objname, filepath, this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader);	
	 
	 MyObjectMgr.SetObjects("jigu");

	 return true;

}
