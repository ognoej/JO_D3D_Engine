#include "ObjectMgr.h"
#include <algorithm>

//ObjectMgr::~ObjectMgr()
//{
//	this->ObjectList->clear();
//	return;
//}

void ObjectMgr::initialize()
{
	findobj = new Object;
	//this->ObjectList = new std::vector<Object>();
	return;
}

void ObjectMgr::UpdateObjList(float dt)
{
	for (int i = 0; i < ObjectList.size(); i++)
	{
		ObjectList[i].UpdateObj(dt);
	}
}

// const string  수정
bool ObjectMgr::AddObject(const std::string _objectname, std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	Object *obj = new Object;
	if (!obj->Initialize(_objectname, filePath, device, deviceContext, cb_vs_vertexshader))
		return false;


	// 복사값?
	ObjectList.push_back(*obj);
	return true;

}

void ObjectMgr::DrawObjects(const XMMATRIX & viewProjectionMatrix)
{
	if (ObjectList.empty())
		return;
	
	std::for_each(ObjectList.begin(), ObjectList.end(), [&](Object i)
	{
		i.Draw(viewProjectionMatrix);
	});
	
}

// 오브젝트 찾기
Object* ObjectMgr::GetObjects(std::string _name)
{

	for (int i = 0; i < ObjectList.size(); i++)
	{
		if (ObjectList[i].ObjectName == _name)
		{
			return &ObjectList[i]; 
		}
	}

	return nullptr;
}

void ObjectMgr::SetObjects(std::string _name)
{
	for (int i = 0; i < ObjectList.size(); i++)
	{
		if (ObjectList[i].ObjectName == _name)
		{
			findobj = &ObjectList[i];
			return;
		}
	}
}


