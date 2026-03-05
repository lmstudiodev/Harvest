#include "Model.h"

bool Model::Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cbvsVertexshader)
{
	this->m_device = device;
	this->m_context = context;
	this->m_texture = texture;
	this->cb_vs_vertexshader = &cbvsVertexshader;

    try
    {
        if (!this->LoadModel(filepath))
            return false;
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

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

void Model::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionmatrix)
{
	this->cb_vs_vertexshader->data.mat = worldMatrix * viewProjectionmatrix;
	this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);
	this->cb_vs_vertexshader->ApplyChanges();

	this->m_context->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());
	this->m_context->PSSetShaderResources(0, 1, &this->m_texture);

    for (int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].Draw();
    }
}