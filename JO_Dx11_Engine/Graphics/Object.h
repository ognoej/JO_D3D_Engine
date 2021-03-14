#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "MyModel.h"

using namespace DirectX;


class Object
{
private:
public:
	bool Initialize(const std::string _name, std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader> & cb_vs_vertexshader);

	//~Object();
	void Draw(const XMMATRIX & viewProjectionMatrix);

	std::string ObjectName;

	XMVECTOR & GetPositionVector();
	XMFLOAT3 & GetPositionFloat3();
	XMMATRIX & GetWorldFloat3();
	const XMVECTOR & GetRotationVector() const;
	const XMFLOAT3 & GetRotationFloat3() const;

	void SetPosition(const XMVECTOR & pos);
	void SetPosition(const XMFLOAT3 & pos);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRev(float x, float y, float z);

	void SetParents(XMFLOAT3& pos);
	void SetParents(XMMATRIX& pos);
	void SetParents(float x, float y, float z);

	void AdjustPosition(const XMVECTOR & pos);
	void AdjustPosition(const XMFLOAT3 & pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR & rot);
	void SetRotation(const XMFLOAT3 & rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR & rot);
	void AdjustRotation(const XMFLOAT3 & rot);

	void AdjustRev(float x, float y, float z);
	void AdjustRev(const XMFLOAT3 &rev);

	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(XMFLOAT3 lookAtPos);
	const XMVECTOR & GetForwardVector();
	const XMVECTOR & GetRightVector();
	const XMVECTOR & GetBackwardVector();
	const XMVECTOR & GetLeftVector();


	MyModel *model = nullptr;

private :

	//MyModel model;
	// squre 나 다각형 모델용 클래스 추가해야함

	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;
	XMFLOAT3 rev;
	XMFLOAT3 *parents = nullptr;
	XMMATRIX *parentsWorld = nullptr;



	void UpdateWorldMatrix();
	XMMATRIX worldMatrix = XMMatrixIdentity();


	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;
};

