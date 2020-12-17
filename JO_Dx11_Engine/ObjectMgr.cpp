#include "ObjectMgr.h"
#include <algorithm>

void ObjectMgr::initialize()
{
	this->ObjectList = new std::vector<Object>();
	return;
}

// const string  수정
Object& ObjectMgr::AddObject(const std::string _objectname, std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	Object obj;
	if(!obj.Initialize(_objectname,filePath,device,deviceContext,cb_vs_vertexshader))
	return;


	// 복사값?
	ObjectList->push_back(obj);
	return obj;

}

void ObjectMgr::DrawObjects(const XMMATRIX & viewProjectionMatrix)
{
	if (ObjectList->empty())
		return;
	std::for_each(ObjectList->begin(), ObjectList->end(), [&](Object i)
	{
		i.Draw(viewProjectionMatrix);
	});
}




Object & ObjectMgr::GetObject(std::string _name)
{
	// 람다식 foreach (시작 , 끝, [가져올 외부 변수](탐색할변수)  가져올 외부변수에 &면 모두 레퍼런스 =면 모두 값 이름이면 해당변수만
	std::for_each(ObjectList->begin(), ObjectList->end(), [_name](Object i)
	{

		if (i.ObjectName == _name)
		{
			return i;
		}
		
	});
}


