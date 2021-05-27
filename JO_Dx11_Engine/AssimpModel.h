#pragma once

#ifndef MODEL_LOADER
#define MODEL_LOADER

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

using namespace DirectX;


class AssimpModel;

#define MAX_BONES 64



// 확인 필요
XMMATRIX toMat4(aiMatrix4x4* ai)
{
	XMFLOAT4X4 temp;
	temp._11 = ai->a1; temp._21 = ai->a2; temp._31 = ai->a3; temp._41 = ai->a4;
	temp._12 = ai->b1; temp._22 = ai->b2; temp._32 = ai->b3; temp._42 = ai->b4;
	temp._13 = ai->c1; temp._23 = ai->c2; temp._33 = ai->c3; temp._43 = ai->c4;
	temp._14 = ai->d1; temp._24 = ai->d2; temp._34 = ai->d3; temp._44 = ai->d4;

	return XMLoadFloat4x4(&temp);
}


struct Mesh
{
	std::vector <XMFLOAT3> vertices;
	std::vector <unsigned int> indices;
	std::vector <XMFLOAT2> uvs;
	std::vector <XMFLOAT3> normals;
	std::vector <XMFLOAT4> weights;
	std::vector <XMFLOAT4> boneID;

	XMMATRIX baseModelMatrix;

	UINT vao, vbo, ebo, uvb, tex, wbo, idbo; // 루트서명
	int posAttribute, texAttribute, weightAttribute, boneAttribute; // 루트서명키
	UINT modelID, viewID, projectionID, transID, modelTransID;

	int width, height;
	unsigned char* image;
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

		XMMATRIX modelTrans;
		//UINT modelTransID; moved to mesh
		void setModelTrans(XMMATRIX);

		std::string rootPath;

		//todo: make init and render optional
		std::vector <Mesh> meshes;
		UINT shader;
		bool modelLoaded;
		AssimpModel(const char* vertfp, const char* fragfp);
		void setShader(const char* vertfp, const char* fragfp);
		void init();
		void render(float dt,XMMATRIX _world, XMMATRIX _viewproj);
	
		class ModelLoader
		{
		private:
			void processNode(const aiScene* scene, aiNode* node, AssimpModel* m);
			void processMesh(const aiScene* scene, aiNode* node, aiMesh* mesh, AssimpModel* m);
			void processAnimations(const aiScene* scene, AssimpModel* m);

		public:
			// this will load all of the required data and dump it into the model struct
			bool loadModel(const char* fp, AssimpModel* m);
		};

	
};

#endif