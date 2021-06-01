#pragma once
#include "MyIndexBuffer.h"
#include "MyVertexBuffer.h"
#include "MyConstBuffer.h"
#include "MyTexture.h"
#include <d3d11.h>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/scene.h>


using namespace DirectX;
#define NUM_BONES_PER_VEREX 4	// ���� �� ���� �޴� �� ����


// �� �������� ������ ���ؽ� ����ü
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	// ���ؽ� ���̴����� MIPS4�� �ִ��� Ȱ���� �� �ֵ��� XMFLOLAT ������ �ڷ����� �ִ��� Ȱ���ϴ� ���� ����.

	XMFLOAT3 pos;			// ���� ��ǥ
	XMFLOAT2 texCoord;		// �ؽ�����ǥ
	XMFLOAT3 Normal;		// ����
	XMFLOAT4 Weights;		// �� ����ġ ������ ����ġ�� 1-����������ġ�� ����� �� �����Ƿ� �����Ͽ� ������� ���� x,y,z�� �ϳ��� ����ġ�� ���
	UINT BoneIndices[4];		// ���� ��ġ�� �� assimp �� ���� 8������ �ҷ���
};





class MyMesh
{
public:
	MyMesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex> & vertices, std::vector<DWORD> & indices, std::vector<MyTexture> & textures);
	MyMesh(const MyMesh & mesh);

	void meshupdate(std::vector<XMMATRIX> finaltrans, ID3D11Device * device);

	void Draw();
	//std::vector<BoneInfo> mMeshBones;
	std::vector<Vertex> mvertices;
	MyVertexBuffer<Vertex> vertexbuffer;


private:
	//std::vector<Vertex> mvertices;
	//MyVertexBuffer<Vertex> vertexbuffer;
	MyIndexBuffer indexbuffer;
	ID3D11DeviceContext * deviceContext;
	std::vector<MyTexture> textures;

	ID3D11Device * mdevice;


};