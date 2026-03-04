#ifndef Model_h__
#define Model_h__
#include "BufferFactory.h"
#include "Vertex.h"
#include "ConstantBuffer.h"

using namespace DirectX;

class Model
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cbvsVertexshader);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const XMMATRIX& viewProjectionmatrix);


private:
	void UpdateWorldMatrix();

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_context = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* cb_vs_vertexshader = nullptr;
	ID3D11ShaderResourceView* m_texture = nullptr;

	BufferFactory<Vertex> m_vertexBuffer;
	BufferFactory<DWORD> m_indexBuffer;

	XMMATRIX m_worldMatrix = XMMatrixIdentity();
};

#endif

