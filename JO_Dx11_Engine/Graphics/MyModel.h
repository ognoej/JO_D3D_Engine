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


	//뼈 하나당 키프레임 포지션값
	std::vector<Keyframe> Keyframes;

};

struct AnimationClip
{
	float GetClipStartTime()const;
	float GetClipEndTime()const;

	void Interpolate(float t, std::vector<XMFLOAT4X4>& boneTransforms)const;

	//뼈 갯수만큼 본 애니메이션
	std::vector<BoneAnimation> BoneAnimations;
};




class MyModel
{
public:
	bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
	void Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix);
	void Release();


	std::vector<MyMesh> meshes;							// 정점 정보
	std::map<std::string, AnimationClip> mAnimations;	// 모델 애니메이션

	UINT mNumAnimationClips = 0;
	UINT mNumBones = 0;
	UINT mNumMeshes = 0;


private:
	const aiScene* pScene;

	void LoadaiMatrixto4x4float(XMFLOAT4X4 &dest, aiMatrix4x4 &src);
	bool LoadModel(const std::string & filePath);
	void LoadAnimation(const aiScene* pScene);

	void ProcessNode(aiNode * node, const aiScene * scene);
	void LoadBonesAndHierarchy(const aiMesh* pMesh,const aiScene* scene, std::vector<BoneInfo>& meshBones);
	MyMesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	std::vector<MyTexture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
	int GetTextureIndex(aiString* pStr);

	ID3D11Device * device = nullptr;
	ID3D11DeviceContext * deviceContext = nullptr;
	MyConstBuffer<CB_VS_vertexshader> * cb_vs_vertexshader = nullptr;
	std::string directory = "";

};