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
#define NUM_BONES_PER_VEREX 4	// 정점 당 영향 받는 뼈 개수


// 본 정보까지 포함한 버텍스 구조체
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	// 버텍스 쉐이더에서 MIPS4를 최대한 활용할 수 있도록 XMFLOLAT 형식의 자료형을 최대한 활용하는 것이 좋다.

	XMFLOAT3 pos;			// 정점 좌표
	XMFLOAT2 texCoord;		// 텍스쳐좌표
	XMFLOAT3 Normal;		// 법선
	XMFLOAT4 TangentU;		// 회전
	XMFLOAT3 Weights;		// 뼈 가중치 마지막 가중치는 1-나머지가중치로 계산할 수 있으므로 생략하여 저장공간 줄임 x,y,z를 하나의 가중치로 계산
	BYTE BoneIndices[4];	// 영향 미치는 뼈 assimp 모델 기준 8개까지 불러옴
};





class MyMesh
{
public:
	MyMesh(ID3D11Device * device, ID3D11DeviceContext * deviceContext, std::vector<Vertex> & vertices, std::vector<DWORD> & indices, std::vector<MyTexture> & textures);
	MyMesh(const MyMesh & mesh);
	void Draw();
	//std::vector<BoneInfo> mMeshBones;

private:
	MyVertexBuffer<Vertex> vertexbuffer;
	MyIndexBuffer indexbuffer;
	ID3D11DeviceContext * deviceContext;
	std::vector<MyTexture> textures;


};