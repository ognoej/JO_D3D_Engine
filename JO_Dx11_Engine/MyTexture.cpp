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
	return aiTextureType();
}

ID3D11ShaderResourceView * MyTexture::GetTextureResourceView()
{
	return nullptr;
}

ID3D11ShaderResourceView ** MyTexture::GetTextureResourceViewAddress()
{
	return nullptr;
}

void MyTexture::Initialize1x1ColorTexture(ID3D11Device * device, const MyColor & colorData, aiTextureType type)
{
}

void MyTexture::InitializeColorTexture(ID3D11Device * device, const MyColor * colorData, UINT width, UINT height, aiTextureType type)
{
}
