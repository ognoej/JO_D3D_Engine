#include "shaders.h"

// blob 으로 읽는 방식을 사용함 blob은 바이너리로 읽어들이는 파일 객체 집합 binary large object
bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT numElements)
{

	HRESULT hr = D3DCompileFromFile(
		L"vertexshader.hlsl",					//	파일명
		nullptr, nullptr,
		"main",						//	정점 셰이더 함수명
		"vs_5_0",						//	셰이더 모델 버전
		D3DCOMPILE_ENABLE_STRICTNESS,	//	컴파일 방법
		0,								//	옵션 정보, 0
		&shader_buffer,
		nullptr);							//	에러 내용 확인





	//HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
	//if (FAILED(hr))
	//{
	//	std::wstring errorMsg = L"Failed to load shader: ";
	//	errorMsg += shaderpath;
	//	//ErrorLogger::Log(hr, errorMsg);
	//	return false;
	//}

	hr = device->CreateVertexShader(this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create vertex shader: ";
		errorMsg += shaderpath;
		//ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreateInputLayout(layoutDesc, numElements, this->shader_buffer->GetBufferPointer(), this->shader_buffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		//::Log(hr, "Failed to create input layout.");
		return false;
	}

	return true;

//	ID3DBlob	*pBlob = nullptr;	//	셰이더 파일용 버퍼
//	ID3DBlob	*pErr = nullptr;	//	에러 메시지 저장용
//
//auto hr = D3DCompileFromFile(
//	L"Sample2.fx",					//	파일명
//	nullptr, nullptr,
//	"VS_PT",						//	정점 셰이더 함수명
//	"vs_4_0",						//	셰이더 모델 버전
//	D3DCOMPILE_ENABLE_STRICTNESS,	//	컴파일 방법
//	0,								//	옵션 정보, 0
//	&pBlob,
//	&pErr);							//	에러 내용 확인
//
//
//	device->CreateVertexShader(
//	pBlob->GetBufferPointer(),
//	pBlob->GetBufferSize(),
//	nullptr,
//	&shader);
//
//if (FAILED(hr))
//{
//	pBlob->Release();
//	return false;
//}
//
//hr = device->CreateInputLayout(layoutDesc, numElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), this->inputLayout.GetAddressOf());
//if (FAILED(hr))
//{
//	//ErrorLogger::Log(hr, "Failed to create input layout.");
//	return false;
//}
//
//pBlob->Release();
//return true;
}

ID3D11VertexShader * VertexShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob * VertexShader::GetBuffer()
{
	return this->shader_buffer.Get();
}

ID3D11InputLayout * VertexShader::GetInputLayout()
{
	return this->inputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
{


	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), this->shader_buffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += shaderpath;
		//ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	hr = device->CreatePixelShader(this->shader_buffer.Get()->GetBufferPointer(), this->shader_buffer.Get()->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create pixel shader: ";
		errorMsg += shaderpath;
		//ErrorLogger::Log(hr, errorMsg);
		return false;
	}

	return true;





//	ID3DBlob	*pBlob = nullptr;		//	셰이더 파일용 버퍼
//	ID3DBlob	*pErr = nullptr;		//	에러 메시지 저장용
//
//
//	auto hr = D3DCompileFromFile(
//		L"Sample2.fx",					//	파일명
//		nullptr, nullptr,
//		"PS_PT",						//	픽셀 셰이더 함수명
//		"ps_4_0",						//	셰이더 모델 버전
//		D3DCOMPILE_ENABLE_STRICTNESS,	//	컴파일 방법
//		0,								//	옵션 정보, 0
//		&pBlob,
//		&pErr);							//	에러 내용 확인
//
//	if (FAILED(hr))
//		return false;
//
//	hr = device->CreatePixelShader(
//		pBlob->GetBufferPointer(),
//		pBlob->GetBufferSize(),
//		nullptr,
//		&shader);
//
//	pBlob->Release();
//
//	if (FAILED(hr))
//		return false;
//
//	return true;
}

ID3D11PixelShader * PixelShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob * PixelShader::GetBuffer()
{
	return this->shader_buffer.Get();
}
