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


XMFLOAT4X4 MyModel::aiMatrixtoXMFLOAT4X4(aiMatrix4x4 _src)
{
	XMFLOAT4X4 dest;
	LoadaiMatrixto4x4float(dest, _src);
	return dest;
}




void MyModel::LoadaiMatrixto4x4float(XMFLOAT4X4 & dest, aiMatrix4x4 & src)
{
	
	dest._11 = src.a1;
	dest._12 = src.a2;
	dest._13 = src.a3;
	dest._14 = src.a4;
			   
	dest._21 = src.b1;
	dest._22 = src.b2;
	dest._23 = src.b3;
	dest._24 = src.b4;
			   
	dest._31 = src.c1;
	dest._32 = src.c2;
	dest._33 = src.c3;
	dest._34 = src.c4;
			   
	dest._41 = src.d1;
	dest._42 = src.d2;
	dest._43 = src.d3;
	dest._44 = src.d4;

	return;
}

bool MyModel::LoadModel(const std::string & filePath)
{
	this->directory = MyString::GetDirectoryFromPath(filePath);

	Assimp::Importer importer;


	// Assimp 로 모델 불러오기. 인스턴싱을 위해 모델완성되면 싱글톤으로 관리 매서드 추가 필요
	pScene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr)
		return false;

	// 씬->애니메이션->채널->포지션&스케일&로테이션 키프레임에 저장
	// 씬->본->웨이트->버텍스아이디&버텍스가중치를 Mesh->Vertives에서 찾아서 x 내가 만든 버텍스에 저장
	// 본마다 버텍스 웨이트 갯수만큼 반복진행.

	/*
	
	
	본 이름 = 애니메이션 채널명 ( + 채널명스케일 + 채널명로테이션 + 채널명 트렌슬레이션 )
	해당 본 이름의 스케일 로테이션 트렌슬레이션 합체 시켜서 채널명에 합해야함
	빈 이름의 채널명은 존재하면 안되는 노드 (잘못된 파일) FBX에서 주로 나타남
	OBJ는 클립이 따로없음. 키프레임 애니메이션을 수행함. 별도의 키프레임 파일이 존재하며 매우 용량을 많이 쳐먹음
	FBX 나 md5는 뼈애니메이션을 들고있음
	

	
	
	
	
	
	
	*/

	// 애니메이션 클립 개수 저장
	if (pScene->HasAnimations())
	{
		LoadAnimation(pScene);
	}

	this->mAnimations;

	this->ProcessNode(pScene->mRootNode, pScene);

	return true;
}


void MyModel::LoadAnimation(const aiScene* pScene)
{
	this->mNumAnimationClips = pScene->mNumAnimations;
	

	for (int i = 0; i < mNumAnimationClips; i++)
	{
		AnimationClip aniclip;
		float ticktime = 0;

		for (int j = 0; j < pScene->mAnimations[i]->mNumChannels; j++)
		{
			BoneAnimation boneani;


			for (int k = 0; k < pScene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
			{				

				Keyframe bonekeyframe;
				bonekeyframe.Translation.x = pScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x;
				bonekeyframe.Translation.y = pScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y;
				bonekeyframe.Translation.z = pScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z;


				bonekeyframe.Scale.x = pScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x;
				bonekeyframe.Scale.y = pScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y;
				bonekeyframe.Scale.z = pScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z;


				bonekeyframe.RotationQuat.x = pScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x;
				bonekeyframe.RotationQuat.y = pScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y;
				bonekeyframe.RotationQuat.z = pScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z;
				bonekeyframe.RotationQuat.w = pScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w;
				
				if(sizeof( bonekeyframe)==1)
				{
					continue;
				}

				bonekeyframe.TimePos = ticktime;
				boneani.Keyframes.push_back(bonekeyframe);
			}

			ticktime += pScene->mAnimations[i]->mTicksPerSecond;

			aniclip.BoneAnimations.push_back(boneani);

		}

		std::string aniname(pScene->mAnimations[i]->mName.C_Str());
		this->mAnimations.insert(make_pair(aniname, aniclip));
	}
}


const aiNodeAnim* MyModel::FindNodeAnim(const aiAnimation* pAnimation, std::string NodeName)
{
	aiNodeAnim* sibal = pAnimation->mChannels[0];
	return sibal;
}

XMFLOAT4X4 XMFLOAT4X4Multifly(XMFLOAT4X4 a, XMFLOAT4X4 b)
{
	
	XMFLOAT4X4 result;
	result._11 = a._11 * b._11 + a._11 * b._21 + a._13 * b._31 + a._14 * b._41;
	result._12 = a._11 * b._12 + a._11 * b._22 + a._13 * b._32 + a._14 * b._42;
	result._13 = a._11 * b._13 + a._11 * b._23 + a._13 * b._33 + a._14 * b._43;
	result._14 = a._11 * b._14 + a._11 * b._24 + a._13 * b._34 + a._14 * b._44;
	result._21 = a._21 * b._11 + a._21 * b._21 + a._23 * b._31 + a._24 * b._41;
	result._22 = a._21 * b._12 + a._21 * b._22 + a._23 * b._32 + a._24 * b._42;
	result._23 = a._21 * b._13 + a._21 * b._23 + a._23 * b._33 + a._24 * b._43;
	result._24 = a._21 * b._14 + a._21 * b._24 + a._23 * b._34 + a._24 * b._44;
	result._31 = a._31 * b._11 + a._31 * b._21 + a._33 * b._31 + a._34 * b._41;
	result._32 = a._31 * b._12 + a._31 * b._22 + a._33 * b._32 + a._34 * b._42;
	result._33 = a._31 * b._13 + a._31 * b._23 + a._33 * b._33 + a._34 * b._43;
	result._34 = a._31 * b._14 + a._31 * b._24 + a._33 * b._34 + a._34 * b._44;
	result._41 = a._41 * b._11 + a._41 * b._21 + a._43 * b._31 + a._44 * b._41;
	result._42 = a._41 * b._12 + a._41 * b._22 + a._43 * b._32 + a._44 * b._42;
	result._43 = a._41 * b._13 + a._41 * b._23 + a._43 * b._33 + a._44 * b._43;
	result._44 = a._41 * b._14 + a._41 * b._24 + a._43 * b._34 + a._44 * b._44;

	return result;

}


XMFLOAT4X4 scalingMatrix(float _x, float _y, float _z)
{
	XMFLOAT4X4 result = {
		_x , 0 , 0 , 0,
		0 , _y , 0 , 0,
		0 , 0 , _z , 0,
		0 , 0 , 0 ,  1,
	};


	return result;
}


XMFLOAT4X4 translationMatrix(float _x, float _y, float _z)
{
	XMFLOAT4X4 result = {
		1 , 0 , 0 , _x,
		0 , 1 , 0 , _y,
		0 , 0 , 1 , _z,
		0 , 0 , 0 ,  1,
	};


	return result;
}

void loadai3x3matrixtoXMFLOAT4X4(XMFLOAT4X4 &_dest, aiMatrix3x3 _src)
{
	_dest._11 = _src.a1;
	_dest._12 = _src.a2;
	_dest._13 = _src.a3;
	_dest._14 = 0;
	_dest._21 = _src.b1;
	_dest._22 = _src.b2;
	_dest._23 = _src.b3;
	_dest._24 = 0;
	_dest._31 = _src.c1;
	_dest._32 = _src.c2;
	_dest._33 = _src.c3;
	_dest._34 = 0;
	_dest._41 = 0 ;
	_dest._42 = 0 ;
	_dest._43 = 0 ;
	_dest._44 = 1 ;
	

	return;

}


void MyModel::ReadNodeHeirarchy(float AnimationTime, aiNode * pNode, XMFLOAT4X4 ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = pScene->mAnimations[0];

	
	XMFLOAT4X4 NodeTransformation(aiMatrixtoXMFLOAT4X4(pNode->mTransformation));

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		//CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		XMFLOAT4X4 ScalingM = scalingMatrix(Scaling.x, Scaling.y, Scaling.z);
		

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		//CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		XMFLOAT4X4 RotationM;
		loadai3x3matrixtoXMFLOAT4X4(RotationM, RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		//CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		XMFLOAT4X4 TranslationM = translationMatrix(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		NodeTransformation = XMFLOAT4X4Multifly(XMFLOAT4X4Multifly(TranslationM , RotationM), ScalingM);
	}

	XMFLOAT4X4 GlobalTransformation = XMFLOAT4X4Multifly (ParentTransform , NodeTransformation);

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		UINT BoneIndex = m_BoneMapping[NodeName];
		Boneinfoes[BoneIndex].FinalTransform = XMFLOAT4X4Multifly(XMFLOAT4X4Multifly(m_GlobalInverseTransform , GlobalTransformation),
			Boneinfoes[BoneIndex].BoneOffsets);
	}

	for (UINT i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}

}


XMFLOAT4X4 float4x4idendity()
{
	XMFLOAT4X4 V = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};
	return V;
}


// 뼈 트랜스폼 갱신
void MyModel::BoneTransform(string nowanimation, float dt)
{
		XMFLOAT4X4 identity = float4x4idendity();

		// animations[0]을 차후에 nowanimation과 이름 대조로 찾아내서 사용해야함.

		float TicksPerSecond = pScene->mAnimations[0]->mTicksPerSecond != 0 ?
			pScene->mAnimations[0]->mTicksPerSecond : 25.0f;
		float TimeInTicks = dt * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, pScene->mAnimations[0]->mDuration);

		//ReadNodeHeirarchy(AnimationTime, pScene->mRootNode, identity);

		finaltransforms.resize(mNumBones);

		for (UINT i = 0; i < mNumBones; i++) {
			finaltransforms[i] = Boneinfoes[i].FinalTransform;
		}
		return;
}


#include <assimp/scene.h>


#include <algorithm>
using namespace std;

void MyModel::LoadBonesAndHierarchy(const aiMesh* mesh,const aiScene* scene, std::vector<BoneInfo>& meshBones)
{

	aiNode* rootnode = scene->mRootNode;

	for (int i = 0; i < mesh->mNumBones; i++)
	{
		BoneInfo boneinfo;

		// 뼈 오프셋 가져오기
		LoadaiMatrixto4x4float(boneinfo.BoneOffsets, mesh->mBones[i]->mOffsetMatrix);

		// 뼈 이름 가져오기
		boneinfo.BoneName = mesh->mBones[i]->mName.data;
	

		//// 뼈 부모 트랜스폼 가져오기
		//aiNode* findnode = rootnode->FindNode(mesh->mBones[i]->mName);
		//if (findnode->mParent != nullptr)
		//{
		//	LoadaiMatrixto4x4float(boneinfo.ToParentTransform, findnode->mParent->mTransformation);
		//}
	
		meshBones.push_back(boneinfo);
	}

	//for (int i = 1; i < mesh->mNumBones; i++)
	//{
	//	// 자식 노드 찾기
	//	aiNode* findnode = rootnode->FindNode(mesh->mBones[i]->mName);
	//	if (findnode->mParent != nullptr)
	//	{
	//		LoadaiMatrixto4x4float( findnode->mTransformation
	//
	//	}
	//	//// 부모 노드 찾기
	//	//for (int j = 0; j < mesh->mNumBones; j++)
	//	//{
	//	//	if (meshBones[j].BoneName == parentnodeName)
	//	//	{
	//	//		meshBones[i].ParentNode = &meshBones[j];
	//	//		break;
	//	//	}
	//	//}
	//
	//}

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

	// 정점 & 본 불러오기 (메쉬 당)
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// 위치 정보 가져오기
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		// 법선 벡터 가져오기
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;
	
	//	// 회전값 가져오기 오류남 코드 수정 필요
	//	vertex.TangentU.x = mesh->mTangents[i].x;
	//	vertex.TangentU.y = mesh->mTangents[i].y;
	//	vertex.TangentU.z = mesh->mTangents[i].z;


		// 메쉬 텍스쳐 불러오기 텍스쳐 혼합을 위해 추가 텍스쳐 작업 코드 필요
		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}


	//메쉬 본과 하이라키 불러오기
	this->LoadBonesAndHierarchy(mesh, scene, meshBones);



	// 정점 인덱스는 뼈에서 지정하는 인덱스와 맞춰야 하므로 Assimp인덱스를 맞춰서 저장한다.
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	Boneinfoes = meshBones;

	std::vector<MyTexture> textures;

	// 현 메쉬의 머터리얼 저장. 물리맵 노말맵 따로 불러와 저장하는 코드 추가 요구됨
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 머터리얼의 텍스쳐 뽑아오기.
	std::vector<MyTexture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);

	// 현 메쉬 텍스쳐들 추가
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	// 불러온 메쉬 리턴
	return MyMesh(this->device, this->deviceContext, vertices, indices, textures);
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
