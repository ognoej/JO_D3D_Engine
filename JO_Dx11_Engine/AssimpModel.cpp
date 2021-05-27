#include "AssimpModel.h"

void Animation::buildBoneTree(const aiScene * scene, aiNode * node, BoneNode * bNode, AssimpModel * m)
{
	if (scene->HasAnimations())
	{
		// found the node
		if (m->boneID.find(node->mName.data) != m->boneID.end())
		{
			std::cout << "Found a bone node: " << node->mName.data << std::endl;
			BoneNode tempNode;
			tempNode.name = node->mName.data;
			tempNode.parent = bNode;
			tempNode.nodeTransform = toMat4(&node->mTransformation);
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

	animations[currentAnim].boneTrans[boneID[node->name]] = finalModel * animations[currentAnim].boneOffset[node->name];

	// loop through every child and use this bone's transformations as the parent transform
	for (int x = 0; x < node->children.size(); x++)
	{
		updateBoneTree(timeInTicks, &node->children[x], finalModel);
	}
}

void AssimpModel::setModelTrans(XMMATRIX)
{
}

AssimpModel::AssimpModel(const char * vertfp, const char * fragfp)
{
}

void AssimpModel::setShader(const char * vertfp, const char * fragfp)
{
}

void AssimpModel::init()
{
}

void AssimpModel::render(float dt, XMMATRIX _world, XMMATRIX _viewproj)
{
}


void AssimpModel::ModelLoader::processNode(const aiScene * scene, aiNode * node, AssimpModel * m)
{
}

void AssimpModel::ModelLoader::processMesh(const aiScene * scene, aiNode * node, aiMesh * mesh, AssimpModel * m)
{
}

void AssimpModel::ModelLoader::processAnimations(const aiScene * scene, AssimpModel * m)
{
}

bool AssimpModel::ModelLoader::loadModel(const char * fp, AssimpModel * m)
{
	return false;
}
