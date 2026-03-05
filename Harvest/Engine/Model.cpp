#include "Model.h"

bool Model::Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_vertexshader>& cbvsVertexshader)
{
    this->m_device = device;
    this->m_context = context;
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

std::vector<Texture> Model::LoadMaterialTexture (aiMaterial* pMaterial, aiTextureType textureType, const aiScene* scene)
{
    std::vector<Texture> materialTextures;

    TextureStorageType storeType = TextureStorageType::Invalid;

    unsigned int textureCount = pMaterial->GetTextureCount(textureType);

    if (textureCount == 0)
    {
        storeType = TextureStorageType::None;
        aiColor3D aiColor(0.0f, 0.0f, 0.0f);

        switch (textureType)
        {
        case aiTextureType_DIFFUSE:
            pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);

            if (aiColor.IsBlack())
            {
                materialTextures.push_back(Texture(m_device, Colors::UnloadedTextureColor, textureType));
                return materialTextures;
            }

            materialTextures.push_back(Texture(m_device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
            return materialTextures;
        }
    }
    else
    {
        materialTextures.push_back(Texture(m_device, Colors::UnhandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
        return materialTextures;
    }
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

    std::vector<Texture> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseTextures = LoadMaterialTexture(material, aiTextureType::aiTextureType_DIFFUSE, scene);

    textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        
    return Mesh(this->m_device, this->m_context, vertices, indices, textures);
}

void Model::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionmatrix)
{
	this->cb_vs_vertexshader->data.mat = worldMatrix * viewProjectionmatrix;
	this->cb_vs_vertexshader->data.mat = XMMatrixTranspose(this->cb_vs_vertexshader->data.mat);
	this->cb_vs_vertexshader->ApplyChanges();

	this->m_context->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader->GetAddressOf());

    for (int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].Draw();
    }
}