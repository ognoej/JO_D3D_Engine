#include "MyTexture.h"


MyTexture::MyTexture(ID3D11Device * device, const MyColor & color, aiTextureType type)
{
	this->Initialize1x1ColorTexture(device, color, type);
}

MyTexture::MyTexture(ID3D11Device * device, const MyColor * colorData, UINT width, UINT height, aiTextureType type)
{
	this->InitializeColorTexture(device, colorData, width, height, type);

}

MyTexture::MyTexture(ID3D11Device * device, const std::string & filePath, aiTextureType type)
{
	this->type = type;
	if (MyString::GetFileExtension(filePath) == ".dds")
	{
		HRESULT hr = DirectX::CreateDDSTextureFromFile(device, MyString::toWstring(filePath).c_str(), texture.GetAddressOf(), this->textureView.GetAddressOf());
		if (FAILED(hr))
		{
			this->Initialize1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
		}
		return;
	}
	else
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, MyString::toWstring(filePath).c_str(), texture.GetAddressOf(), this->textureView.GetAddressOf());
		if (FAILED(hr))
		{
			this->Initialize1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
		}
		return;
	}
}

MyTexture::MyTexture(ID3D11Device * device, const uint8_t * pData, size_t size, aiTextureType type)
{
	this->type = type;
	HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, this->texture.GetAddressOf(), this->textureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create Texture from memory.");
}

aiTextureType MyTexture::GetType()
{
	return this->type;
}

ID3D11ShaderResourceView * MyTexture::GetTextureResourceView()
{
	return this->textureView.Get();
}

ID3D11ShaderResourceView ** MyTexture::GetTextureResourceViewAddress()
{
	return this->textureView.GetAddressOf();
}

void MyTexture::Initialize1x1ColorTexture(ID3D11Device * device, const MyColor & colorData, aiTextureType type)
{
	InitializeColorTexture(device, &colorData, 1, 1, type);
}

void MyTexture::InitializeColorTexture(ID3D11Device * device, const MyColor * colorData, UINT width, UINT height, aiTextureType type)
{
	this->type = type;
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D * p2DTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = colorData;
	initialData.SysMemPitch = width * sizeof(MyColor);
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
	COM_ERROR_IF_FAILED(hr, "Failed to initialize texture from color data.");
	texture = static_cast<ID3D11Texture2D*>(p2DTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from texture generated from color data.");
}
