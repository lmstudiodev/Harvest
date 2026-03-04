#include "Model.h"

bool Model::Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cbvsVertexshader)
{
	this->m_device = device;
	this->m_context = context;
	this->m_texture = texture;
	this->cb_vs_vertexshader = &cbvsVertexshader;

    if (!this->LoadModel(filepath))
        return false;

    this->SetPosition(0.0f, 0.0f, 0.0f);
    this->SetRotation(0.0f, 0.0f, 0.0f);

	UpdateWorldMatrix();

	return true;
}

bool Model::LoadModel(const std::string& filepath)
{
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
        return false;
    
    this->ProcessNode(pScene->mRootNode, pScene);

    return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(this->ProcessMesh(mesh, scene));
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;

    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

        if (mesh->mTextureCoords[0])
        {
            vertex.textCoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.textCoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(this->m_device, this->m_context, vertices, indices);
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

    for (int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].Draw();
    }

	//this->m_context->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//UINT offset = 0;

	//this->m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePointer(), &offset);
	//this->m_context->DrawIndexed(this->m_indexBuffer.BufferSize(), 0, 0);
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