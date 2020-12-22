#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/material.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "MyColor.h"
#include "..\Utilites\MyErrorCheck.h"
#include "..\Utilites\MyString.h"


// 텍스쳐 타입용 열거체
enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

class MyTexture
{
public:
	MyTexture(ID3D11Device * device, const MyColor & color, aiTextureType type);
	MyTexture(ID3D11Device * device, const MyColor * colorData, UINT width, UINT height, aiTextureType type); //Generate texture of specific color data
	MyTexture(ID3D11Device * device, const std::string & filePath, aiTextureType type);
	MyTexture(ID3D11Device * device, const uint8_t* pData, size_t size, aiTextureType type);
	aiTextureType GetType();
	ID3D11ShaderResourceView * GetTextureResourceView();
	ID3D11ShaderResourceView ** GetTextureResourceViewAddress();

private:
	void Initialize1x1ColorTexture(ID3D11Device * device, const MyColor & colorData, aiTextureType type);
	void InitializeColorTexture(ID3D11Device * device, const MyColor * colorData, UINT width, UINT height, aiTextureType type);
	Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;

};

