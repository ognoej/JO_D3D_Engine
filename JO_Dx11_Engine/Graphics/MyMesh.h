#pragma once
#include "MyIndexBuffer.h"
#include "MyVertexBuffer.h"
#include "MyConstBuffer.h"
#include "MyTexture.h"
#include <d3d11.h>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;

	// 본과 노말벡터 추가해야함
};

class MyMesh
{
public:
	MyMesh(ID3D11Device * device, aiBone** _bones, ID3D11DeviceContext * deviceContext, std::vector<Vertex> & vertices, std::vector<DWORD> & indices, std::vector<MyTexture> & textures);
	MyMesh(const MyMesh & mesh);
	void Draw();
	aiBone** bones;
private:
	MyVertexBuffer<Vertex> vertexbuffer;
	MyIndexBuffer indexbuffer;
	ID3D11DeviceContext * deviceContext;
	std::vector<MyTexture> textures;
};

