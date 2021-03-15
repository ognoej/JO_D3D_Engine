#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MyMesh.h"
#include <map>
#include "MathHelper.h"

using namespace DirectX;

struct Keyframe
{
	Keyframe();
	~Keyframe();

	float TimePos;
	XMFLOAT3 Translation;
	XMFLOAT3 Scale;
	XMFLOAT4 RotationQuat;
};

struct BoneAnimation
{
	float GetStartTime()const;
	float GetEndTime()const;

	void Interpolate(float t, XMFLOAT4X4& M)const;

	std::vector<Keyframe> Keyframes;

};

struct AnimationClip
{
	float GetClipStartTime()const;
	float GetClipEndTime()const;

	void Interpolate(float t, std::vector<XMFLOAT4X4>& boneTransforms)const;

	std::vector<BoneAnimation> BoneAnimations;
};



class MyModel
{
public:
	bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
	void Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix);
	void Release();
	void AddRef();

	std::vector<MyMesh> meshes;

private:
	bool LoadModel(const std::string & filePath);
	void ProcessNode(aiNode * node, const aiScene * scene);
	MyMesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	std::vector<MyTexture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
	int GetTextureIndex(aiString* pStr);

	ID3D11Device * device = nullptr;
	ID3D11DeviceContext * deviceContext = nullptr;
	MyConstBuffer<CB_VS_vertexshader> * cb_vs_vertexshader = nullptr;
	std::string directory = "";


	// 애니메이션 변수
	std::map<std::string, AnimationClip> mAnimations;

};
