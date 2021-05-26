#include "MyMesh.h"

MyMesh::MyMesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext,std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<MyTexture>& textures)
{
	this->deviceContext = deviceContext;
	this->textures = textures;
	//this->bones = _bones;


	//this->mdevice = device;
	//this->mvertices = vertices;
	//this->mvertices.clear();

	//mvertices.assign(vertices.begin(), vertices.end());

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
	this->mvertices = mesh.mvertices;
}


// cpu에서 파이널트랜스폼 적용해보기 코드
void MyMesh::meshupdate(std::vector<XMMATRIX> finaltrans, ID3D11Device * device)
{

		for (int i = 0; i < this->mvertices.size(); i++)
		{
		
			XMFLOAT3 posP = { 0, 0, 0 };

			XMFLOAT4 holy(mvertices[i].pos.x, mvertices[i].pos.y, mvertices[i].pos.z,1);
			
			float posx = mvertices[i].pos.x * finaltrans[this->mvertices[i].BoneIndices[0]].r->m128_f32[0];
			float posy = mvertices[i].pos.y * finaltrans[this->mvertices[i].BoneIndices[0]].r->m128_f32[1];
			float posz = mvertices[i].pos.z * finaltrans[this->mvertices[i].BoneIndices[0]].r->m128_f32[2];

			XMFLOAT3 tempos = { posx, posy, posz };

			posP.x += (this->mvertices[i].Weights.x * tempos.x);
			posP.y += (this->mvertices[i].Weights.x * tempos.y);
			posP.z += (this->mvertices[i].Weights.x * tempos.z);


			posx = mvertices[i].pos.x * finaltrans[this->mvertices[i].BoneIndices[1]].r->m128_f32[0];
			posy = mvertices[i].pos.y * finaltrans[this->mvertices[i].BoneIndices[1]].r->m128_f32[1];
			posz = mvertices[i].pos.z * finaltrans[this->mvertices[i].BoneIndices[1]].r->m128_f32[2];

			tempos = { posx, posy, posz };

			posP.x += (this->mvertices[i].Weights.y * tempos.x);
			posP.y += (this->mvertices[i].Weights.y * tempos.y);
			posP.z += (this->mvertices[i].Weights.y * tempos.z);


			posx = mvertices[i].pos.x * finaltrans[this->mvertices[i].BoneIndices[2]].r->m128_f32[0];
			posy = mvertices[i].pos.y * finaltrans[this->mvertices[i].BoneIndices[2]].r->m128_f32[1];
			posz = mvertices[i].pos.z * finaltrans[this->mvertices[i].BoneIndices[2]].r->m128_f32[2];

			tempos = { posx, posy, posz };

			posP.x += (this->mvertices[i].Weights.z * tempos.x);
			posP.y += (this->mvertices[i].Weights.z * tempos.y);
			posP.z += (this->mvertices[i].Weights.z * tempos.z);

			posx = mvertices[i].pos.x * finaltrans[this->mvertices[i].BoneIndices[3]].r->m128_f32[0];
			posy = mvertices[i].pos.y * finaltrans[this->mvertices[i].BoneIndices[3]].r->m128_f32[1];
			posz = mvertices[i].pos.z * finaltrans[this->mvertices[i].BoneIndices[3]].r->m128_f32[2];

			tempos = { posx, posy, posz };

			posP.x += (this->mvertices[i].Weights.w * tempos.x);
			posP.y += (this->mvertices[i].Weights.w * tempos.y);
			posP.z += (this->mvertices[i].Weights.w * tempos.z);


			mvertices[i].pos = posP;
		}

		mvertices;
		HRESULT hr = this->vertexbuffer.Initialize(device, mvertices.data(), mvertices.size());
		//COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");
		
	//output.outPosition = mul(float4(posL, 1.0f), mat);
	

		
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
