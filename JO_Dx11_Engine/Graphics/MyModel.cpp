#include "MyModel.h"

bool MyModel::Initialize(const std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->cb_vs_vertexshader = &cb_vs_vertexshader;

	try
	{
		if (!this->LoadModel(filePath))
			return false;
	}
	catch (MyErrorCheck & exception)
	{
		MyErrorCheck::Log(exception);
		return false;
	}

	return true;
}

void MyModel::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	this->cb_vs_vertexshader->data.mat = worldMatrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);
	this->cb_vs_vertexshader->ApplyChanges();
	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw();
	}
}

void MyModel::Release()
{
	this->meshes.clear();
	return;
}



bool MyModel::LoadModel(const std::string & filePath)
{
	this->directory = MyString::GetDirectoryFromPath(filePath);

	Assimp::Importer importer;


	// Assimp �� �� �ҷ�����. �ν��Ͻ��� ���� �𵨿ϼ��Ǹ� �̱������� ���� �ż��� �߰� �ʿ�
	const aiScene* pScene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr)
		return false;



	// ��->�ִϸ��̼�->ä��->������&������&�����̼� Ű�����ӿ� ����
	// ��->��->����Ʈ->���ؽ����̵�&���ؽ�����ġ�� Mesh->Vertives���� ã�Ƽ� x ���� ���� ���ؽ��� ����
	// ������ ���ؽ� ����Ʈ ������ŭ �ݺ�����.


	// �ִϸ��̼� Ŭ�� ���� ����
	if (pScene->HasAnimations())
	{
		this->mNumAnimationClips = pScene->mNumAnimations;
	}

	this->ProcessNode(pScene->mRootNode, pScene);

	return true;
}


#include <algorithm>
using namespace std;

void MyModel::LoadBonesAndHierarchy(const aiMesh* mesh,const aiScene* scene, std::vector<BoneInfo>& meshBones)
{
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		BoneInfo boneinfo;
		boneinfo.BoneOffsets._11 = mesh->mBones[i]->mOffsetMatrix.a1;
		boneinfo.BoneOffsets._12 = mesh->mBones[i]->mOffsetMatrix.a2;
		boneinfo.BoneOffsets._13 = mesh->mBones[i]->mOffsetMatrix.a3;
		boneinfo.BoneOffsets._14 = mesh->mBones[i]->mOffsetMatrix.a4;

		boneinfo.BoneOffsets._21 = mesh->mBones[i]->mOffsetMatrix.b1;
		boneinfo.BoneOffsets._22 = mesh->mBones[i]->mOffsetMatrix.b2;
		boneinfo.BoneOffsets._23 = mesh->mBones[i]->mOffsetMatrix.b3;
		boneinfo.BoneOffsets._24 = mesh->mBones[i]->mOffsetMatrix.b4;

		boneinfo.BoneOffsets._31 = mesh->mBones[i]->mOffsetMatrix.c1;
		boneinfo.BoneOffsets._32 = mesh->mBones[i]->mOffsetMatrix.c2;
		boneinfo.BoneOffsets._33 = mesh->mBones[i]->mOffsetMatrix.c3;
		boneinfo.BoneOffsets._34 = mesh->mBones[i]->mOffsetMatrix.c4;

		boneinfo.BoneOffsets._41 = mesh->mBones[i]->mOffsetMatrix.d1;
		boneinfo.BoneOffsets._42 = mesh->mBones[i]->mOffsetMatrix.d2;
		boneinfo.BoneOffsets._43 = mesh->mBones[i]->mOffsetMatrix.d3;
		boneinfo.BoneOffsets._44 = mesh->mBones[i]->mOffsetMatrix.d4;

		boneinfo.BoneHierarchy = mesh->mBones[i]->mName.C_Str();

		//std::vector<BoneInfo>::iterator iter;
		//iter = std::find(meshBones.begin(), meshBones.end(), scene->mRootNode->FindNode(mesh->mBones[i]->mName));
		
		//auto it = find(meshBones.begin(), meshBones.end(), scene->mRootNode->FindNode(mesh->mBones[i]->mName));
		//
		//boneinfo.ParentNode = &meshBones[it - meshBones.begin()];
		// if (it == meshBones.end())
		// {
		//	 meshBones.push_back(boneinfo);
		//	 continue;
		// }
		//else 
		//{
		//}
	}

	return;
}




void MyModel::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}


MyMesh MyModel::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	std::vector<BoneInfo> meshBones;

	// ���� & �� �ҷ����� (�޽� ��)
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// ��ġ ���� ��������
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		// ���� ���� ��������
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;
	
	//	// ȸ���� �������� ������ �ڵ� ���� �ʿ�
	//	vertex.TangentU.x = mesh->mTangents[i].x;
	//	vertex.TangentU.y = mesh->mTangents[i].y;
	//	vertex.TangentU.z = mesh->mTangents[i].z;


		// �޽� �ؽ��� �ҷ����� �ؽ��� ȥ���� ���� �߰� �ؽ��� �۾� �ڵ� �ʿ�
		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}


	//�޽� ���� ���̶�Ű �ҷ�����
	this->LoadBonesAndHierarchy(mesh, scene, meshBones);



	// ���� �ε����� ������ �����ϴ� �ε����� ����� �ϹǷ� Assimp�ε����� ���缭 �����Ѵ�.
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	std::vector<MyTexture> textures;

	// �� �޽��� ���͸��� ����. ������ �븻�� ���� �ҷ��� �����ϴ� �ڵ� �߰� �䱸��
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// ���͸����� �ؽ��� �̾ƿ���.
	std::vector<MyTexture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);

	// �� �޽� �ؽ��ĵ� �߰�
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	// �ҷ��� �޽� ����
	return MyMesh(this->device, this->deviceContext, meshBones, vertices, indices, textures);
}

TextureStorageType MyModel::DetermineTextureStorageType(const aiScene * pScene, aiMaterial * pMat, unsigned int index, aiTextureType textureType)
{
	if (pMat->GetTextureCount(textureType) == 0)
		return TextureStorageType::None;

	aiString path;
	pMat->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();

	//Check if texture is an embedded indexed texture by seeing if the file path is an index #
	if (texturePath[0] == '*')
	{
		if (pScene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}
	//Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
	{
		if (pTex->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}
	//Lastly check if texture is a filepath by checking for period before extension name
	if (texturePath.find('.') != std::string::npos)
	{
		return TextureStorageType::Disk;
	}

	return TextureStorageType::None; // No texture exists
}

std::vector<MyTexture> MyModel::LoadMaterialTextures(aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene)
{
	std::vector<MyTexture> materialTextures;
	TextureStorageType storetype = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);

	if (textureCount == 0) //If there are no textures
	{
		storetype = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (textureType)
		{
		case aiTextureType_DIFFUSE:
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack()) //If color = black, just use grey
			{
				materialTextures.push_back(MyTexture(this->device, Colors::UnloadedTextureColor, textureType));
				return materialTextures;
			}
			materialTextures.push_back(MyTexture(this->device, MyColor(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
			return materialTextures;
		}
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			pMaterial->GetTexture(textureType, i, &path);
			TextureStorageType storetype = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
			switch (storetype)
			{
			case TextureStorageType::EmbeddedIndexCompressed:
			{
				int index = GetTextureIndex(&path);
				MyTexture embeddedIndexedTexture(this->device,
					reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
					pScene->mTextures[index]->mWidth,
					textureType);
				materialTextures.push_back(embeddedIndexedTexture);
				break;
			}
			case TextureStorageType::EmbeddedCompressed:
			{
				const aiTexture * pTexture = pScene->GetEmbeddedTexture(path.C_Str());
				MyTexture embeddedTexture(this->device,
					reinterpret_cast<uint8_t*>(pTexture->pcData),
					pTexture->mWidth,
					textureType);
				materialTextures.push_back(embeddedTexture);
				break;
			}
			case TextureStorageType::Disk:
			{
				std::string filename = this->directory + '\\' + path.C_Str();
				MyTexture diskTexture(this->device, filename, textureType);
				materialTextures.push_back(diskTexture);
				break;
			}
			}
		}
	}

	if (materialTextures.size() == 0)
	{
		materialTextures.push_back(MyTexture(this->device, Colors::UnhandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
	}
	return materialTextures;
}

int MyModel::GetTextureIndex(aiString * pStr)
{
	assert(pStr->length >= 2);
	return atoi(&pStr->C_Str()[1]);
}

Keyframe::Keyframe()
	: TimePos(0.0f),
	Translation(0.0f, 0.0f, 0.0f),
	Scale(1.0f, 1.0f, 1.0f),
	RotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Keyframe::~Keyframe()
{
}

float BoneAnimation::GetStartTime() const
{
	// Keyframes are sorted by time, so first keyframe gives start time.
	return Keyframes.front().TimePos;
}

float BoneAnimation::GetEndTime() const
{
	// Keyframes are sorted by time, so last keyframe gives end time.
	float f = Keyframes.back().TimePos;

	return f;
}

void BoneAnimation::Interpolate(float t, XMFLOAT4X4 & M) const
{
	if (t <= Keyframes.front().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&Keyframes.front().Scale);
		XMVECTOR P = XMLoadFloat3(&Keyframes.front().Translation);
		XMVECTOR Q = XMLoadFloat4(&Keyframes.front().RotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else if (t >= Keyframes.back().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&Keyframes.back().Scale);
		XMVECTOR P = XMLoadFloat3(&Keyframes.back().Translation);
		XMVECTOR Q = XMLoadFloat4(&Keyframes.back().RotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else
	{
		for (UINT i = 0; i < Keyframes.size() - 1; ++i)
		{
			if (t >= Keyframes[i].TimePos && t <= Keyframes[i + 1].TimePos)
			{
				float lerpPercent = (t - Keyframes[i].TimePos) / (Keyframes[i + 1].TimePos - Keyframes[i].TimePos);

				XMVECTOR s0 = XMLoadFloat3(&Keyframes[i].Scale);
				XMVECTOR s1 = XMLoadFloat3(&Keyframes[i + 1].Scale);

				XMVECTOR p0 = XMLoadFloat3(&Keyframes[i].Translation);
				XMVECTOR p1 = XMLoadFloat3(&Keyframes[i + 1].Translation);

				XMVECTOR q0 = XMLoadFloat4(&Keyframes[i].RotationQuat);
				XMVECTOR q1 = XMLoadFloat4(&Keyframes[i + 1].RotationQuat);

				XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
				XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
				XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);

				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));

				break;
			}
		}
	}
}



float AnimationClip::GetClipStartTime() const
{
	// Find smallest start time over all bones in this clip.
	float t = MathHelper::Infinity;
	for (UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = MathHelper::Min(t, BoneAnimations[i].GetStartTime());
	}

	return t;
}

float AnimationClip::GetClipEndTime() const
{
	// Find largest end time over all bones in this clip.
	float t = 0.0f;
	for (UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = MathHelper::Max(t, BoneAnimations[i].GetEndTime());
	}

	return t;
}

void AnimationClip::Interpolate(float t, std::vector<XMFLOAT4X4>& boneTransforms) const
{
	for (UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		BoneAnimations[i].Interpolate(t, boneTransforms[i]);
	}
}
