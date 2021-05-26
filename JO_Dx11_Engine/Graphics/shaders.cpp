#include "shaders.h"

// blob ���� �д� ����� ����� blob�� ���̳ʸ��� �о���̴� ���� ��ü ���� binary large object
bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT numElements)
{

	HRESULT hr = D3DCompileFromFile(
		L"vertexshader.hlsl",					//	���ϸ�
		nullptr, nullptr,
		"main",						//	���� ���̴� �Լ���
		"vs_5_0",						//	���̴� �� ����
		D3DCOMPILE_ENABLE_STRICTNESS,	//	������ ���
		0,								//	�ɼ� ����, 0
		&shader_buffer,
		nullptr);							//	���� ���� Ȯ��





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

//	ID3DBlob	*pBlob = nullptr;	//	���̴� ���Ͽ� ����
//	ID3DBlob	*pErr = nullptr;	//	���� �޽��� �����
//
//auto hr = D3DCompileFromFile(
//	L"Sample2.fx",					//	���ϸ�
//	nullptr, nullptr,
//	"VS_PT",						//	���� ���̴� �Լ���
//	"vs_4_0",						//	���̴� �� ����
//	D3DCOMPILE_ENABLE_STRICTNESS,	//	������ ���
//	0,								//	�ɼ� ����, 0
//	&pBlob,
//	&pErr);							//	���� ���� Ȯ��
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





//	ID3DBlob	*pBlob = nullptr;		//	���̴� ���Ͽ� ����
//	ID3DBlob	*pErr = nullptr;		//	���� �޽��� �����
//
//
//	auto hr = D3DCompileFromFile(
//		L"Sample2.fx",					//	���ϸ�
//		nullptr, nullptr,
//		"PS_PT",						//	�ȼ� ���̴� �Լ���
//		"ps_4_0",						//	���̴� �� ����
//		D3DCOMPILE_ENABLE_STRICTNESS,	//	������ ���
//		0,								//	�ɼ� ����, 0
//		&pBlob,
//		&pErr);							//	���� ���� Ȯ��
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
