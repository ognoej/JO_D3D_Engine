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
	//pScene = 
	// ReadFile 한 후에 임포터는 importer::GetOrphanedScene()으로 마지막에 리드한 파일의 권한을 넘겨주고 해제시켜야함.
	// 그렇지 않으면 임포터가 할당 해제되면서 읽어온 파일도 릴리즈 하고 없어지게됨. 출처 Assimp홈페이지

		importer.ReadFile(_fullpath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	

	if (pScene == nullptr)
	{
		return false;
	}

	m_aiScenes.insert(std::make_pair(_modelname, importer.GetOrphanedScene()));

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