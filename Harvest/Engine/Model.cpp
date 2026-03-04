#include "Model.h"

bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cbvsVertexshader)
{
	this->m_device = device;
	this->m_context = context;
	this->m_texture = texture;
	this->cb_vs_vertexshader = &cbvsVertexshader;

    try
    {
        Vertex vertices[] =
        {
            Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f), //FRONT BOTTOM LEFT [0]
                Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f), //FRONT TOP LEFT [1]
                Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f), //FRONT TOP RIGHT [2]
                Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f), //FRONT BOTTOM RIGHT [3]

                Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f), //BACK BOTTOM LEFT [4]
                Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f), //BACK TOP LEFT [5]
                Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f), //BACK TOP RIGHT [6]
                Vertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f), //BACK BOTTOM RIGHT [7]
        };

        HRESULT hr = m_vertexBuffer.Initialize(this->m_device, vertices, ARRAYSIZE(vertices));
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to create vertex buffer.");

        DWORD indexes[] =
        {
            0, 1, 2, //FRONT
            0, 2, 3, //FRONT
            4, 7, 6, //BACK
            4, 6, 5, //BACK
            3, 2, 6, //RIGHT SIDE
            3, 6, 7, //RIGHT SIDE
            4, 5, 1, //LEFT SIDE
            4, 1, 0, //LEFT SIDE
            1, 5, 6, //TOP
            1, 6, 2, //TOP
            0, 3, 7, //BOTTOM
            0, 7, 4 //BOTTOM
        };

        hr = m_indexBuffer.Initialize(this->m_device, indexes, ARRAYSIZE(indexes), false);
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to create index buffer.");

    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(0.0f, 0.0f, 0.0f);

	UpdateWorldMatrix();

	return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
	m_texture = texture;
}

void Model::Draw(const XMMATRIX& viewProjectionmatrix)
{
	this->cb_vs_vertexshader->data.mat = this->m_worldMatrix * viewProjectionmatrix;
	this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);
	this->cb_vs_vertexshader->ApplyChanges();

	this->m_context->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());
	this->m_context->PSSetShaderResources(0, 1, &this->m_texture);
	this->m_context->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT offset = 0;

	this->m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePointer(), &offset);
	this->m_context->DrawIndexed(this->m_indexBuffer.BufferSize(), 0, 0);
}

void Model::UpdateWorldMatrix()
{
    this->m_worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
    XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);

    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

const XMVECTOR& Model::GetPositionVector() const
{
    return this->posVector;
}

const XMFLOAT3& Model::GetPositionFloat3() const
{
    return this->pos;
}

const XMVECTOR& Model::GetRotationVector() const
{
    return this->rotVector;
}

const XMFLOAT3& Model::GetRotationFloat3() const
{
    return this->rot;
}

void Model::SetPosition(const XMVECTOR& pos)
{
    XMStoreFloat3(&this->pos, pos);
    this->posVector = pos;
    this->UpdateWorldMatrix();
}

void Model::SetPosition(const XMFLOAT3& pos)
{
    this->pos = pos;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void Model::SetPosition(float x, float y, float z)
{
    this->pos = XMFLOAT3(x, y, z);
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMVECTOR& pos)
{
    this->posVector += pos;
    XMStoreFloat3(&this->pos, this->posVector);
    this->UpdateWorldMatrix();
}

void Model::AdjustPosition(const XMFLOAT3& pos)
{
    this->pos.x += pos.x;
    this->pos.y += pos.y;
    this->pos.z += pos.z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void Model::AdjustPosition(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void Model::SetRotation(const XMVECTOR& rot)
{
    XMStoreFloat3(&this->rot, rot);
    this->rotVector = rot;
    this->UpdateWorldMatrix();
}

void Model::SetRotation(const XMFLOAT3& rot)
{
    this->rot = rot;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void Model::SetRotation(float x, float y, float z)
{
    this->rot = XMFLOAT3(x, y, z);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMVECTOR& rot)
{
    this->rotVector += rot;
    XMStoreFloat3(&this->rot, this->rotVector);
    this->UpdateWorldMatrix();
}

void Model::AdjustRotation(const XMFLOAT3& rot)
{
    this->rot.x += rot.x;
    this->rot.y += rot.y;
    this->rot.z += rot.z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void Model::AdjustRotation(float x, float y, float z)
{
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

const XMVECTOR& Model::GetForwardVector()
{
    return this->vec_forward;
}

const XMVECTOR& Model::GetRightVector()
{
    return this->vec_right;
}

const XMVECTOR& Model::GetBackwardVector()
{
    return this->vec_backward;
}

const XMVECTOR& Model::GetLeftVector()
{
    return vec_left;
}