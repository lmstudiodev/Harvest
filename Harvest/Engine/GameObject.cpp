#include "GameObject.h"

bool GameObject::Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_vertexshader>& cbvsVertexshader)
{
	if (!m_model.Initialize(filepath, device, context, cbvsVertexshader))
		return false;

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);

	UpdateWorldMatrix();

	return true;
}

void GameObject::UpdateWorldMatrix()
{
	this->m_worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);

	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

void GameObject::Draw(const XMMATRIX& viewProjectionmatrix)
{
	m_model.Draw(this->m_worldMatrix, viewProjectionmatrix);
}

const XMVECTOR& GameObject::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& GameObject::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR& GameObject::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3& GameObject::GetRotationFloat3() const
{
	return this->rot;
}

void GameObject::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMVECTOR& pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMFLOAT3& pos)
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMVECTOR& rot)
{
	XMStoreFloat3(&this->rot, rot);
	this->rotVector = rot;
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMFLOAT3& rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMVECTOR& rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

const XMVECTOR& GameObject::GetForwardVector()
{
	return this->vec_forward;
}

const XMVECTOR& GameObject::GetRightVector()
{
	return this->vec_right;
}

const XMVECTOR& GameObject::GetBackwardVector()
{
	return this->vec_backward;
}

const XMVECTOR& GameObject::GetLeftVector()
{
	return vec_left;
}