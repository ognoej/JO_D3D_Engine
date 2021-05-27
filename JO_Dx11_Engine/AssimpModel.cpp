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
}

void AssimpModel::updateBoneTree(double time, Animation::BoneNode * node, XMMATRIX transform)
{
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
