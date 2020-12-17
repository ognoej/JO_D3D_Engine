#include "ObjectMgr.h"
#include <algorithm>

void ObjectMgr::initialize()
{
	this->ObjectList = new std::vector<Object>();
	return;
}

// const string  ����
Object& ObjectMgr::AddObject(const std::string _objectname, std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	Object obj;
	if(!obj.Initialize(_objectname,filePath,device,deviceContext,cb_vs_vertexshader))
	return;


	// ���簪?
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
	// ���ٽ� foreach (���� , ��, [������ �ܺ� ����](Ž���Һ���)  ������ �ܺκ����� &�� ��� ���۷��� =�� ��� �� �̸��̸� �ش纯����
	std::for_each(ObjectList->begin(), ObjectList->end(), [_name](Object i)
	{

		if (i.ObjectName == _name)
		{
			return i;
		}
		
	});
}


