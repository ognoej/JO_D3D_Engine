#pragma once
#include "MyIndexBuffer.h"
#include "MyVertexBuffer.h"
#include "MyConstBuffer.h"
#include "MyTexture.h"
#include <d3d11.h>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
using namespace DirectX;


struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	XMFLOAT3 pos;			// 정점 좌표
	XMFLOAT2 texCoord;		// 텍스쳐좌표
	XMFLOAT3 Normal;		// 법선
	XMFLOAT4 TangentU;		// 회전
	XMFLOAT3 Weights;		// 뼈 가중치
	BYTE BoneIndices[8];	// 영향 미치는 뼈 assimp 모델 기준 8개까지 불러옴
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

