#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "MyModel.h"

using namespace DirectX;


class Object
{
private :
	MyModel model;
	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;
	XMFLOAT3 rev;
	XMFLOAT3 *parents = nullptr;
	XMMATRIX *parentsWorld = nullptr;	

};

