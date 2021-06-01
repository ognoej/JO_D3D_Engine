#pragma once

#ifndef _ASSIMPMODEL_H_
#define _ASSIMPMODEL_H_

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <d3d11.h>
#include <DirectXMath.h>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <Windows.h>
#include "../Graphics/MyTexture.h"
#include "Graphics/MyConstBuffer.h"
#include "Graphics/MyVertexBuffer.h"
#include "Graphics/MyIndexBuffer.h"


using namespace DirectX;


class AssimpModel;


struct Vertexassimp
{
	Vertexassimp() {}
	Vertexassimp(float x, float y, float z, float u, float v)
		: pos(x, y, z), uvs(u, v) {}

	// ���ؽ� ���̴����� MIPS4�� �ִ��� Ȱ���� �� �ֵ��� XMFLOLAT ������ �ڷ����� �ִ��� Ȱ���ϴ� ���� ����.

	XMFLOAT3 pos;			// ���� ��ǥ
	XMFLOAT2 uvs;			// �ؽ�����ǥ
	XMFLOAT3 normals;		// ����
	XMFLOAT4 weights;		// �� ����ġ ������ ����ġ�� 1-����������ġ�� ����� �� �����Ƿ� �����Ͽ� ������� ���� x,y,z�� �ϳ��� ����ġ�� ���
	XMFLOAT4 boneID;		// ���� ��ġ�� �� assimp �� ���� 8������ �ҷ���
};



struct Mesh
{
	std::vector <XMFLOAT3> vertices;
	std::vector <DWORD> indices;
	std::vector <XMFLOAT2> uvs;
	std::vector <XMFLOAT3> normals;
	std::vector <XMFLOAT4> weights;
	std::vector <XMFLOAT4> boneID;

	XMMATRIX baseModelMatrix;

	UINT vao, vbo, ebo, uvb, tex, wbo, idbo; // ��Ʈ����
	int posAttribute, texAttribute, weightAttribute, boneAttribute; // ��Ʈ����
	UINT modelID, viewID, projectionID, transID, modelTransID; // object id

	unsigned int numVertices;
	int width, height;
	std::vector<MyTexture> image;
	std::vector<Vertexassimp> vertives;

	MyVertexBuffer<Vertexassimp> vertexbuffer;
	MyIndexBuffer indexbuffer;

public : 
	void Draw(ID3D11DeviceContext * deviceContext);
	void makebuffers(ID3D11Device* device);
};

struct Animation
{
	std::string name;
	double duration;
	double ticksPerSecond;
	// all of the bone transformations, this is modified every frame
	// assimp calls it a channel, its anims for a node aka bone
	std::vector <XMMATRIX> boneTrans;
	std::map <std::string, XMMATRIX> boneOffset;

	struct Channel
	{
		std::string name;
		XMMATRIX offset;
		std::vector <aiVectorKey> mPositionKeys;
		std::vector <aiQuatKey> mRotationKeys;
		std::vector <aiVectorKey> mScalingKeys;
	};
	std::vector <Channel> channels;

	struct BoneNode
	{
		std::string name;
		BoneNode* parent;
		std::vector <BoneNode> children;
		XMMATRIX nodeTransform;
		XMMATRIX boneTransform;
	};
	BoneNode root;

	void buildBoneTree(const aiScene* scene, aiNode* node, BoneNode* bNode, AssimpModel* m);

	//aiAnimation* data;
};


class AssimpModel
{
public:
		// all of the animations
		std::vector<Animation> animations;
		unsigned int currentAnim;
		void setAniamtion(std::string name);
		std::vector <std::string> animNames;
		// map of bones
		std::map <std::string, unsigned int> boneID;
		// runs every frame
		void tick(double time);
		void updateBoneTree(double time, Animation::BoneNode* node, XMMATRIX transform);


		ID3D11Device * device = nullptr;
		ID3D11DeviceContext * deviceContext = nullptr;
		MyConstBuffer<CB_VS_vertexshader> * cb_vs_vertexshader = nullptr;


		XMMATRIX modelTrans;
		//UINT modelTransID; moved to mesh
		void setModelTrans(XMMATRIX);



		std::string rootPath;

		//todo: make init and render optional
		std::vector <Mesh> meshes;
		UINT shader;
		bool modelLoaded;
		AssimpModel();
		void setShader(const char* vertfp, const char* fragfp);
		void init(ID3D11Device* _device, ID3D11DeviceContext* _devicecontext);
		void render(float dt,XMMATRIX _world, XMMATRIX _viewproj);

		//std::vector<MyTexture> textures;
	
		
};

class ModelLoder
{
private:
	void processNode(const aiScene* scene, aiNode* node, AssimpModel* m ,ID3D11Device* device);
	void processMesh(const aiScene* scene, aiNode* node, aiMesh* mesh, AssimpModel* m ,ID3D11Device* device);
	void processAnimations(const aiScene* scene, AssimpModel* m);
	std::vector<MyTexture> LoadMaterialTextures(aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene, ID3D11Device* device);

public:
	// this will load all of the required data and dump it into the model struct
	bool loadModel(std::string fp, AssimpModel* m, ID3D11Device* device);
};
#endif