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

	XMFLOAT3 pos;			// ���� ��ǥ
	XMFLOAT2 texCoord;		// �ؽ�����ǥ
	XMFLOAT3 Normal;		// ����
	XMFLOAT4 TangentU;		// ȸ��
	XMFLOAT3 Weights;		// �� ����ġ
	BYTE BoneIndices[8];	// ���� ��ġ�� �� assimp �� ���� 8������ �ҷ���
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

