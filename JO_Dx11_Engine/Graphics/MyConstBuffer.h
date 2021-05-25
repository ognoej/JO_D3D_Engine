#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__
#include <DirectXMath.h>
#include "..\Utilites\MyErrorCheck.h"
#include <wrl/client.h>

struct CB_VS_vertexshader
{
	DirectX::XMMATRIX mat;
	DirectX::XMMATRIX gWorldView;
	DirectX::XMMATRIX gWorldViewProj;
	DirectX::XMMATRIX gBoneTransforms[69];
};

struct CB_PS_pixelshader
{
	// 알파값
	float alpha = 1.0f;
};


// 상수버퍼 템플릿
template<class T>
class MyConstBuffer
{
private:
	MyConstBuffer(const MyConstBuffer<T>& rhs);

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext * deviceContext = nullptr;

public:
	MyConstBuffer() {}

	T data;

	ID3D11Buffer* Get()const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()const
	{
		return buffer.GetAddressOf();
	}

	HRESULT Initialize(ID3D11Device *device, ID3D11DeviceContext * deviceContext)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, buffer.GetAddressOf());
		return hr;
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			MyErrorCheck::Log(hr, "Failed to map constant buffer.");
			return false;
		}
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		this->deviceContext->Unmap(buffer.Get(), 0);
		return true;
	}

};



#endif // ConstantBuffer_h__