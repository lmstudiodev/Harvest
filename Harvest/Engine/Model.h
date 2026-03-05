#ifndef Model_h__
#define Model_h__
#include "Mesh.h"

using namespace DirectX;

class Model
{
public:
	bool Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<CB_VS_vertexshader>& cbvsVertexshader);
	void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionmatrix);

private:
	bool LoadModel(const std::string& filepath);
	std::vector<Texture> LoadMaterialTexture(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* scene);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh*mesh , const aiScene* scene);

private:
	std::vector<Mesh> m_meshes;

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_context = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* cb_vs_vertexshader = nullptr;
};

#endif

