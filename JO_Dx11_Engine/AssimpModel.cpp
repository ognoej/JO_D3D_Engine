#include "AssimpModel.h"

// 확인 필요 DirectX와 Opengl 오더 다름
XMMATRIX toXMMAT(aiMatrix4x4* ai)
{
	XMFLOAT4X4 temp;
	temp._11 = ai->a1; temp._21 = ai->a2; temp._31 = ai->a3; temp._41 = ai->a4;
	temp._12 = ai->b1; temp._22 = ai->b2; temp._32 = ai->b3; temp._42 = ai->b4;
	temp._13 = ai->c1; temp._23 = ai->c2; temp._33 = ai->c3; temp._43 = ai->c4;
	temp._14 = ai->d1; temp._24 = ai->d2; temp._34 = ai->d3; temp._44 = ai->d4;

	return XMLoadFloat4x4(&temp);
}

void Animation::buildBoneTree(const aiScene * scene, aiNode * node, BoneNode * bNode, AssimpModel * m)
{
	if (scene->HasAnimations())
	{
		// found the node
		if (m->boneID.find(node->mName.data) != m->boneID.end())
		{
			//std::cout << "Found a bone node: " << node->mName.data << std::endl;
			BoneNode tempNode;
			tempNode.name = node->mName.data;
			tempNode.parent = bNode;
			tempNode.nodeTransform = toXMMAT(&node->mTransformation);
			// bones and their nodes always share the same name
			tempNode.boneTransform = boneOffset[tempNode.name];
			bNode->children.push_back(tempNode);
		}

		if (node->mNumChildren > 0)
		{
			for (unsigned int x = 0; x < node->mNumChildren; x++)
			{
				// if the node we just found was a bone node then pass it in (current bone node child vector size - 1)
				if (m->boneID.find(node->mName.data) != m->boneID.end())
					buildBoneTree(scene, node->mChildren[x], &bNode->children[bNode->children.size() - 1], m);
				else
					buildBoneTree(scene, node->mChildren[x], bNode, m);
			}
		}
	}
}

void AssimpModel::setAniamtion(std::string name)
{
	


}

void AssimpModel::tick(double time)
{
	double timeInTicks = time * animations[currentAnim].ticksPerSecond;

	// opengl glm::mat4(1.0f) = 단위행렬 == XMMatrixidentity()
	updateBoneTree(timeInTicks, &animations[currentAnim].root, XMMatrixIdentity());
}

void AssimpModel::updateBoneTree(double timeInTicks, Animation::BoneNode * node, XMMATRIX parentTransform)
{
	int chanIndex = 0;
	for (int x = 0; x < animations[currentAnim].channels.size(); x++)
	{
		if (node->name == animations[currentAnim].channels[x].name)
		{
			chanIndex = x;
		}
	}

	double animTime = std::fmod(timeInTicks, animations[currentAnim].duration);

	aiQuaternion aiRotation(animations[currentAnim].channels[chanIndex].mRotationKeys[0].mValue);
	aiVector3D aiTranslation(animations[currentAnim].channels[chanIndex].mPositionKeys[0].mValue);
	aiVector3D aiScale(animations[currentAnim].channels[chanIndex].mScalingKeys[0].mValue);

	Assimp::Interpolator<aiQuaternion> slerp;
	Assimp::Interpolator<aiVector3D> lerp;

	// get the two animation keys it is between for lerp and slerp
	int key1, key2;
	if (std::round(animTime) < animTime)
	{
		key1 = std::round(animTime); key2 = key1 + 1;
	}
	else
	{
		key1 = std::round(animTime) - 1; key2 = std::round(animTime);
	}

	if (animations[currentAnim].channels[chanIndex].mPositionKeys.size() > 1)
		lerp(aiTranslation, animations[currentAnim].channels[chanIndex].mPositionKeys[key1].mValue, animations[currentAnim].channels[chanIndex].mPositionKeys[key2].mValue, animTime - key1); // translation
	if (animations[currentAnim].channels[chanIndex].mScalingKeys.size() > 1)
		lerp(aiScale, animations[currentAnim].channels[chanIndex].mScalingKeys[key1].mValue, animations[currentAnim].channels[chanIndex].mScalingKeys[key2].mValue, animTime - key1); // scale
	if (animations[currentAnim].channels[chanIndex].mRotationKeys.size() > 1)
		slerp(aiRotation, animations[currentAnim].channels[chanIndex].mRotationKeys[key1].mValue, animations[currentAnim].channels[chanIndex].mRotationKeys[key2].mValue, animTime - key1); // rotation

	XMFLOAT3 translation((float)aiTranslation.x, (float)aiTranslation.y, (float)aiTranslation.z);
	XMFLOAT3 scaling((float)aiScale.x, (float)aiScale.y, (float)aiScale.z);
	XMVECTOR rotation = { (float)aiRotation.w, (float)aiRotation.x, (float)aiRotation.y, (float)aiRotation.z };


	// aimatrix4x4 to XMmatrix 수정 필요
	XMMATRIX finalModel =
			DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z)
		*	DirectX::XMMatrixRotationQuaternion(rotation)
		*	DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z)
		*	parentTransform ;

	//순서 바꿔야함 오프셋 * 파이널
	animations[currentAnim].boneTrans[boneID[node->name]] = animations[currentAnim].boneOffset[node->name] * finalModel;

	// loop through every child and use this bone's transformations as the parent transform
	for (int x = 0; x < node->children.size(); x++)
	{
		updateBoneTree(timeInTicks, &node->children[x], finalModel);
	}
}

void AssimpModel::setModelTrans(XMMATRIX in)
{
	modelTrans = in;
}




std::vector<MyTexture> ModelLoder::LoadMaterialTextures(aiMaterial * pMaterial, aiTextureType textureType, const aiScene * pScene, ID3D11Device* device)
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
				materialTextures.push_back(MyTexture(device, Colors::UnloadedTextureColor, textureType));
				return materialTextures;
			}
			materialTextures.push_back(MyTexture(device, MyColor(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
			return materialTextures;
		}
	}
	/*
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
	*/
	if (materialTextures.size() == 0)
	{
		materialTextures.push_back(MyTexture(device, Colors::UnhandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
	}
	//return materialTextures;
	return materialTextures;
}

AssimpModel::AssimpModel()
{
	//shader = loadShader(vfp, ffp); 쉐이더 설정
	modelLoaded = false;
}

void AssimpModel::setShader(const char * vertfp, const char * fragfp)
{
	//shader = loadShader(vfp, ffp);
}

void AssimpModel::init(ID3D11Device* _device, ID3D11DeviceContext* _devicecontext)
{
	if (!modelLoaded)
	{
		printf("Please load in a model before initializing buffers.");
		return;
	}

	device = _device;
	deviceContext = _devicecontext;
	

}

void AssimpModel::render(float dt, XMMATRIX _world, XMMATRIX _viewproj)
{
	if (!modelLoaded)
	{
		return;
	}

	this->cb_vs_vertexshader->data.mat = _world * _viewproj;
	this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);

	this->cb_vs_vertexshader->data.gWorldView = _world;
	this->cb_vs_vertexshader->data.gWorldView = XMMatrixTranspose(this->cb_vs_vertexshader->data.gWorldView);

	this->cb_vs_vertexshader->data.gWorldViewProj = _world * _viewproj; // XMmatrix => hlsl 행렬로 방향 바꾸기
	this->cb_vs_vertexshader->data.gWorldViewProj = XMMatrixTranspose(this->cb_vs_vertexshader->data.gWorldViewProj);

	for (int i = 0; i < 69; i++)
	{
		//this->cb_vs_vertexshader->data.gBoneTransforms[i] = XMLoadFloat4x4(&Boneinfoes[i].FinalTransform);
		this->cb_vs_vertexshader->data.gBoneTransforms[i] = animations[currentAnim].boneTrans[i];
	}



	this->cb_vs_vertexshader->ApplyChanges(); // 변화 적용



	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(this->deviceContext);
	}

}


void ModelLoder::processNode(const aiScene * scene, aiNode * node, AssimpModel * m, ID3D11Device* device)
{
	//std::cout << "Processing a node: " << node->mName.C_Str() << std::endl; //debug
	   // this is where the fun part starts.

	   // cycle through each mesh within this node
	if (node->mNumMeshes > 0)
	{
		// cycle through each mesh
		for (unsigned int x = 0; x < node->mNumMeshes; x++)
		{

			processMesh(scene, node,
				scene->mMeshes[node->mMeshes[x]], // scene contains all of the meshes, nodes simply have indices into the scene mesh array
				m, device);
		}
	}
	if (m->boneID.find(node->mName.data) != m->boneID.end())
	//	std::cout << node->mName.data << " IS A BONE NODE!!!!";

	// then go through each child in the node and process them as well
	if (node->mNumChildren > 0)
	{
		for (unsigned int x = 0; x < node->mNumChildren; x++)
		{
			processNode(scene, node->mChildren[x], m, device);
		}
	}

}



void ModelLoder::processMesh(const aiScene * scene, aiNode * node, aiMesh * mesh, AssimpModel * m, ID3D11Device* device)
{
	//std::cout << "Processing a mesh: " << mesh->mName.C_Str() << std::endl; //debug

	//std::cout << "Has bones? " << mesh->mNumBones << std::endl;

	Mesh tempMesh;
	tempMesh.weights.resize(mesh->mNumVertices);
	std::fill(tempMesh.weights.begin(), tempMesh.weights.end(), XMFLOAT4(0,0,0,0));
	tempMesh.boneID.resize(mesh->mNumVertices);
	std::fill(tempMesh.boneID.begin(), tempMesh.boneID.end(), XMFLOAT4(-123, -123, -123, -123));
	tempMesh.numVertices = mesh->mNumVertices;


	tempMesh.baseModelMatrix = toXMMAT(&node->mTransformation);
	if (node->mParent != NULL)
		tempMesh.baseModelMatrix = toXMMAT(&node->mParent->mTransformation) * toXMMAT(&node->mTransformation);

	// cycle through each vertex in the mesh
	for (unsigned x = 0; x < mesh->mNumVertices; x++)
	{
		// load the vertices
		XMFLOAT3 tempV;
		tempV.x = mesh->mVertices[x].x;
		tempV.y = mesh->mVertices[x].y;
		tempV.z = mesh->mVertices[x].z;
		tempMesh.vertices.push_back(tempV);

		//	 load the uvs
		XMFLOAT2 tempUV;
		tempUV.x = mesh->mTextureCoords[0][x].x;
		tempUV.y = mesh->mTextureCoords[0][x].y;
		tempMesh.uvs.push_back(tempUV);

		// load the normals (if they exist)
		if (mesh->HasNormals())
		{
			XMFLOAT3 tempN;
			tempN.x = mesh->mNormals[x].x;
			tempN.y = mesh->mNormals[x].y;
			tempN.z = mesh->mNormals[x].z;
			tempMesh.normals.push_back(tempN);
		}
	}

	// cycle through each face to get the indices
	for (unsigned int x = 0; x < mesh->mNumFaces; x++)
	{
		// ALWAYS USE AIPROCESS_TRIANGULATE!!! not doing so will make all of the indices wrong!!!
		tempMesh.indices.push_back(mesh->mFaces[x].mIndices[0]);
		tempMesh.indices.push_back(mesh->mFaces[x].mIndices[1]);
		tempMesh.indices.push_back(mesh->mFaces[x].mIndices[2]);
	}

	if (scene->HasMaterials())
	{
		// so that we don't have to type out that whole thing every time
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		//std::cout << "Has diffuse texture: " << mat->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;

		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			// don't know why you have to get the texture name like this
			aiString path;
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			std::cout << path.C_Str() << std::endl;


			// from model
			std::vector<MyTexture> diffuseTextures = LoadMaterialTextures(mat, aiTextureType::aiTextureType_DIFFUSE, scene ,device);

			// 현 메쉬 텍스쳐들 추가
			//textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());


			tempMesh.image.insert(tempMesh.image.end(), diffuseTextures.begin(), diffuseTextures.end());

		}
	}

	if (mesh->HasBones())
	{
		for (int x = 0; x < mesh->mNumBones; x++)
		{
			// bone index, decides what bone we modify
			unsigned int index = 0;

			if (m->boneID.find(mesh->mBones[x]->mName.data) == m->boneID.end())
			{ // create a new bone
				// current index is the new bone
				index = m->boneID.size();
			}
			else
			{
				index = m->boneID[mesh->mBones[x]->mName.data];
			}

			m->boneID[mesh->mBones[x]->mName.data] = index;

			for (int y = 0; y < m->animations[m->currentAnim].channels.size(); y++)
			{
				if (m->animations[m->currentAnim].channels[y].name == mesh->mBones[x]->mName.data)
					m->animations[m->currentAnim].boneOffset[mesh->mBones[x]->mName.data] = toXMMAT(&mesh->mBones[x]->mOffsetMatrix);
			}


			for (int y = 0; y < mesh->mBones[x]->mNumWeights; y++)
			{
				unsigned int vertexID = mesh->mBones[x]->mWeights[y].mVertexId;
				// first we check if the boneid vector has any filled in
				// if it does then we need to fill the weight vector with the same value
				if (tempMesh.boneID[vertexID].x == -123)
				{
					tempMesh.boneID[vertexID].x = index;
					tempMesh.weights[vertexID].x = mesh->mBones[x]->mWeights[y].mWeight;
				}
				else if (tempMesh.boneID[vertexID].y == -123)
				{
					tempMesh.boneID[vertexID].y = index;
					tempMesh.weights[vertexID].y = mesh->mBones[x]->mWeights[y].mWeight;
				}
				else if (tempMesh.boneID[vertexID].z == -123)
				{
					tempMesh.boneID[vertexID].z = index;
					tempMesh.weights[vertexID].z = mesh->mBones[x]->mWeights[y].mWeight;
				}
				else if (tempMesh.boneID[vertexID].w == -123)
				{
					tempMesh.boneID[vertexID].w = index;
					tempMesh.weights[vertexID].w = mesh->mBones[x]->mWeights[y].mWeight;
				}
			}

		}
	}


	tempMesh.makebuffers(device);


	m->meshes.push_back(tempMesh);


}

void ModelLoder::processAnimations(const aiScene * scene, AssimpModel * m)
{
	if (scene->HasAnimations())
	{
		for (int x = 0; x < scene->mNumAnimations; x++)
		{
			Animation tempAnim;
			tempAnim.name = scene->mAnimations[x]->mName.data;
			tempAnim.duration = scene->mAnimations[x]->mDuration;
			tempAnim.ticksPerSecond = scene->mAnimations[x]->mTicksPerSecond;
			//tempAnim.data = scene->mAnimations[x];

			// load in required data for animation so that we don't have to save the entire scene
			for (int y = 0; y < scene->mAnimations[x]->mNumChannels; y++)
			{
				Animation::Channel tempChan;
				tempChan.name = scene->mAnimations[x]->mChannels[y]->mNodeName.data;

				for (int z = 0; z < scene->mAnimations[x]->mChannels[y]->mNumPositionKeys; z++)
					tempChan.mPositionKeys.push_back(scene->mAnimations[x]->mChannels[y]->mPositionKeys[z]);

				for (int z = 0; z < scene->mAnimations[x]->mChannels[y]->mNumRotationKeys; z++)
					tempChan.mRotationKeys.push_back(scene->mAnimations[x]->mChannels[y]->mRotationKeys[z]);

				for (int z = 0; z < scene->mAnimations[x]->mChannels[y]->mNumScalingKeys; z++)
					tempChan.mScalingKeys.push_back(scene->mAnimations[x]->mChannels[y]->mScalingKeys[z]);

				tempAnim.channels.push_back(tempChan);
			}

			m->currentAnim = 0;
			// 69는 maxbone  scene->mMeshes[0]->mNumBones
			for (int z = 0; z < 69; z++)
			{
				tempAnim.boneTrans.push_back(XMMatrixIdentity());
			}

			m->animations.push_back(tempAnim);
		}

		m->animations[m->currentAnim].root.name = "rootBoneTreeNode";
	}
}

bool ModelLoder::loadModel(std::string fp, AssimpModel * m , ID3D11Device* device)
{
	//log("Loading " + (std::string)fp, green);

	Assimp::Importer importer; // used to import the model

	const aiScene* scene = importer.ReadFile(fp,
		aiProcess_Triangulate |
		aiProcess_OptimizeMeshes |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_PreTransformVertices |
		aiProcess_FlipUVs);

	if (!scene)
	{
		//core::log("Error importing " + (std::string)fp + ": " + importer.GetErrorString(), core::error);
		return false;
	}

	// get the root fp
	m->rootPath = fp;
	for (int x = m->rootPath.size() - 1; x >= 0; x--)
	{
		if (m->rootPath[x] == '/' || m->rootPath[x] == '\\')
		{
			m->rootPath = m->rootPath.substr(0, x + 1);
			x = -1;
		}
	}

	// some debug stuff, delete later
	//std::cout << "Number of total meshes: " << scene->mNumMeshes << std::endl;
	//std::cout << "Animations: " << scene->HasAnimations() << std::endl;

	// set 64 bones to identity, 64 is current limit, might increase it later
	processAnimations(scene, m);

	// start processing the model
	processNode(scene, scene->mRootNode, m, device);

	// must be called after processNode
	if (scene->HasAnimations())
		m->animations[m->currentAnim].buildBoneTree(scene, scene->mRootNode, &m->animations[m->currentAnim].root, m);

	m->modelTrans = XMMatrixIdentity();

	m->modelLoaded = true;

	//core::log((std::string)fp + " loaded successfully.", core::green);
	return true;
}

void Mesh::Draw(ID3D11DeviceContext * deviceContext)
{
	UINT offset = 0;

	for (int i = 0; i < image.size(); i++)
	{
		if (image[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
		{
			deviceContext->PSSetShaderResources(0, 1, image[i].GetTextureResourceViewAddress());
			break;
		}
	}

	deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
	deviceContext->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(this->indexbuffer.IndexCount(), 0, 0);
}

void Mesh::makebuffers(ID3D11Device* device)
{
	
	for (int i = 0; i < numVertices; i++)
	{
		Vertexassimp tempvertex;
		tempvertex.pos = this->vertices[i];
		//tempvertex.normals = this->normals[i];
		tempvertex.uvs = this->uvs[i];
		tempvertex.boneID = this->boneID[i];
		tempvertex.weights = this->weights[i];
		vertives.push_back(tempvertex);
	}

	HRESULT hr = this->vertexbuffer.Initialize(device, vertives.data(), vertives.size());

	hr = this->indexbuffer.Initialize(device, indices.data(), indices.size());

}
