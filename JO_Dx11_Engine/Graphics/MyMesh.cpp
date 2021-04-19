#include "MyMesh.h"

MyMesh::MyMesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext,std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<MyTexture>& textures)
{
	this->deviceContext = deviceContext;
	this->textures = textures;
	//this->bones = _bones;

	HRESULT hr = this->vertexbuffer.Initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");

	hr = this->indexbuffer.Initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
}

MyMesh::MyMesh(const MyMesh & mesh)
{
	this->deviceContext = mesh.deviceContext;
	this->indexbuffer = mesh.indexbuffer;
	this->vertexbuffer = mesh.vertexbuffer;
	this->textures = mesh.textures;
}

void MyMesh::Draw()
{
	UINT offset = 0;

	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
		{
			this->deviceContext->PSSetShaderResources(0, 1, textures[i].GetTextureResourceViewAddress());
			break;
		}
	}

	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	this->deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
}
