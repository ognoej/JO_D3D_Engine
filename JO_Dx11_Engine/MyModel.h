#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MyMesh.h"
using namespace DirectX;



class MyModel
{
public:
	bool Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
	void Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix);

private:
	std::vector<MyMesh> meshes;
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



};

