#pragma once
#include "Object.h"

class ObjectMgr
{
private :
	std::vector<Object> *ObjectList;

public :
	void initialize();
	Object& AddObject(const std::string _objectname, std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);
	void DrawObjects(const XMMATRIX & viewProjectionMatrix);
	Object& GetObject(std::string _name);
	
};

