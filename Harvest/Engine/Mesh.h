#pragma once
#include "BufferFactory.h"
#include "Vertex.h"
#include "ConstantBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Texture.h"

class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures);
	Mesh(const Mesh& mesh);

	void Draw();

private:
	VertexBuffer<Vertex> m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	ID3D11DeviceContext* m_context = nullptr;

	std::vector<Texture> m_textures;
};