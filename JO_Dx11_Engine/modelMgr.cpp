#include "modelMgr.h"
#include "MyString.h"

modelMgr::modelMgr()
{
}

modelMgr::~modelMgr()
{
	Release();
}


bool  modelMgr::LoadaiScene(std::string _fullpath, std::string _modelname)
{
	this->directory = MyString::GetDirectoryFromPath(_fullpath);

	Assimp::Importer importer;


	const aiScene* pScene = new aiScene;

	// Assimp 로 모델 불러오기. 인스턴싱을 위해 모델완성되면 싱글톤으로 관리 매서드 추가 필요
	pScene = importer.ReadFile(_fullpath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr)
	{
		return false;
	}

	m_aiScenes.insert(std::make_pair(_modelname, pScene));

	return true;
}

const aiScene * modelMgr::FindaiScene(std::string _imgName)
{
	auto find = m_aiScenes.find(_imgName);

	if (find == m_aiScenes.end())
		return nullptr;

	return find->second;
}

void modelMgr::Release()
{
	for (auto i : m_aiScenes)
	{
		delete i.second;
	}
	m_aiScenes.clear();
}