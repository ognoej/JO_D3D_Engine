#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include "MathHelper.h"
#include "Singleton.h"


class modelMgr : public TemplateSingletonA<modelMgr>
{
	friend TemplateSingletonA;


private:
	modelMgr();
	~modelMgr();
	void Release();

private:
	std::map<std::string,const aiScene*>	m_aiScenes;
	HWND							m_hWnd;

public:
	inline	void	SetHwnd(HWND _hWnd)
	{
		m_hWnd = _hWnd;
	}
	bool		LoadaiScene(std::string _fullpath, std::string _modelname);
	//aiScene*	CreateaiScene(int _width, int _height, std::string *_imgName = nullptr);

	const aiScene*	FindaiScene(std::string _modelname);
	void	Release();

	std::string directory = "";

};
#define aiSceneMgr		modelMgr::GetInstance()