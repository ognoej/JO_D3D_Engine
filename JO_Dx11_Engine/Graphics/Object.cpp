#include "Object.h"

//Object::~Object()
//{
//	if (this->model != nullptr)
//	{
//		delete(model);
//	}
//	return;
//}

bool Object::Initialize(const std::string _name, std::string & filePath, ID3D11Device * device, ID3D11DeviceContext * deviceContext, MyConstBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	if (filePath == "Sphere")
	{
		return true;
	}
	else if (filePath == "Square")
	{
		return true;
	}
	else
	{
		//���ϸ��ϸ� ��Ʈ ��ġ�� �ٲ� 

		// model = new MyModel(filePath, device, deviceContext, cb_vs_vertexshader);
		model = new MyModel(filePath, device, deviceContext, cb_vs_vertexshader);

		this->ObjectName = _name;
		//model = new MyModel;
		//if (!model->Initialize(filePath, device, deviceContext, cb_vs_vertexshader))
		//	return false;

		model->pScene;

		this->SetPosition(0.0f, 0.0f, 0.0f);
		this->SetRotation(0.0f, 0.0f, 0.0f);
		this->SetScale(1.0f, 1.0f, 1.0f);
		this->SetRev(0.0f, 0.0f, 0.0f);

		this->UpdateWorldMatrix();

		GetFinalTransform(3.0f);


		return true;
	}

}

void Object::UpdateObj(float dt)
{

	
	this->model->updatemodel();
	

	



	//for (int i = 0; i < this->mvertices.size(); i++)
	//{
	//	XMMATRIX posL = XMMatrixIdentity();
	//	XMMATRIX tempos = XMMatrixIdentity();
	//	XMVector3Transform(XMLoadFloat3(&this->mvertices[i].pos), tempos);
	//
	//
	//		posL += this->mvertices[i].Weights.x * (tempos* finaltrans[this->mvertices[i].BoneIndices[0]]);
	//		posL += this->mvertices[i].Weights.y * (tempos* finaltrans[this->mvertices[i].BoneIndices[1]]);
	//		posL += this->mvertices[i].Weights.z * (tempos* finaltrans[this->mvertices[i].BoneIndices[2]]);
	//		posL += this->mvertices[i].Weights.w * (tempos* finaltrans[this->mvertices[i].BoneIndices[3]]);
	//
	//		XMVECTOR temvec = { 0, 0, 0, 0 };
	//
	//}











	//if (nowanimation != "");
	//GetFinalTransform(dt);

}

void Object::Draw(const XMMATRIX & viewProjectionMatrix)
{

	if(model!=nullptr)
	model->Draw(this->worldMatrix, viewProjectionMatrix);
	
}

// ���⺤��
XMVECTOR & Object::GetPositionVector()
{
	return this->posVector;
}

// ���⺤�� float3
XMFLOAT3 & Object::GetPositionFloat3()
{
	return this->pos;
}

 // ������ǥ
XMMATRIX & Object::GetWorldFloat3()
{
	return this->worldMatrix;
}

// ȸ������
const XMVECTOR & Object::GetRotationVector() const
{
	return this->rotVector;
}

// ȸ�� float3
const XMFLOAT3 & Object::GetRotationFloat3() const
{
	return this->rot;
}

// ��ġ ���� vector����
void Object::SetPosition(const XMVECTOR & pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateWorldMatrix();
}


// ��ġ ���� float3
void Object::SetPosition(const XMFLOAT3 & pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

// ��ġ ���� ���� ����
void Object::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

// ũ�� ����
void Object::SetScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
	this->UpdateWorldMatrix();
}

// ���� ����
void Object::SetRev(float x, float y, float z)
{
	this->rev.x = x;
	this->rev.y = y;
	this->rev.z = z;
	this->UpdateWorldMatrix();
}

//�θ� ���� 
void Object::SetParents(XMFLOAT3 & pos)
{
	this->parents = &pos;
	this->UpdateWorldMatrix();
}

// �θ� ����
void Object::SetParents(XMMATRIX & pos)
{
	this->parentsWorld = &pos;
	this->UpdateWorldMatrix();
}

//�θ� ����
void Object::SetParents(float x, float y, float z)
{
	this->parents->x = x;
	this->parents->x = y;
	this->parents->x = z;

}

// ��ġ ����
void Object::AdjustPosition(const XMVECTOR & pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateWorldMatrix();
}

// ��ġ ����
void Object::AdjustPosition(const XMFLOAT3 & pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

// ��ġ ����
void Object::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}


// ȸ�� ���� vector
void Object::SetRotation(const XMVECTOR & rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateWorldMatrix();
}

// ȸ�� ���� float3
void Object::SetRotation(const XMFLOAT3 & rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

// ȸ�� ���� ���� 3
void Object::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

// ȸ�� ����
void Object::AdjustRotation(const XMVECTOR & rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}

void Object::AdjustRotation(const XMFLOAT3 & rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Object::AdjustRev(float x, float y, float z)
{
	this->rev.x += x;
	this->rev.y += y;
	this->rev.z += z;
	this->rotVector = XMLoadFloat3(&this->rev);
	this->UpdateWorldMatrix();
}

void Object::AdjustRev(const XMFLOAT3 & rev)
{
}

void Object::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

// ������Ʈ �ٶ󺸴� Lookat ���� ����
void Object::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	this->SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR & Object::GetForwardVector()
{
	return this->vec_forward;
}

const XMVECTOR & Object::GetRightVector()
{
	return this->vec_right;
}

const XMVECTOR & Object::GetBackwardVector()
{
	return this->vec_backward;
}

const XMVECTOR & Object::GetLeftVector()
{
	return this->vec_left;
}

const void Object::GetFinalTransform(float dt)
{


	model->BoneTransform(nowanimation,dt,finaltransforms);

	return void();
}


// ��� ������Ʈ
void Object::UpdateWorldMatrix()
{
	if (this->parents != nullptr)
	{
		this->worldMatrix = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z)* XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z) *
			XMMatrixRotationRollPitchYaw(this->rev.x, this->rev.y, this->rev.z)* XMMatrixTranslation(this->parents->x, this->parents->y, this->parents->z);
	}

	else if (this->parentsWorld != nullptr)
	{
		this->worldMatrix = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z)* XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z) *
			XMMatrixRotationRollPitchYaw(this->rev.x, this->rev.y, this->rev.z) * *(this->parentsWorld);
	}
   
	else
	{
		this->worldMatrix = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z)* XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z) *
			XMMatrixRotationRollPitchYaw(this->rev.x, this->rev.y, this->rev.z);//*XMMatrixTranslation(this->parents.x,this->parents.y,this->parents.z);
	}

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}