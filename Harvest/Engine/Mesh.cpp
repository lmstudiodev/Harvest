#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
{
	try
	{
		this->m_context = context;
		
		HRESULT hr = m_vertexBuffer.Initialize(device, vertices.data(), vertices.size());
		COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to create vertex buffer for mesh.");

		hr = m_indexBuffer.Initialize(device, indices.data(), indices.size());
		COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to create index buffer for mesh.");
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
	}
}

Mesh::Mesh(const Mesh& mesh)
{
	this->m_context = mesh.m_context;
	this->m_vertexBuffer = mesh.m_vertexBuffer;
	this->m_indexBuffer = mesh.m_indexBuffer;
}

void Mesh::Draw()
{
	UINT offset = 0;

	this->m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePointer(), &offset);
	this->m_context->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	this->m_context->DrawIndexed(this->m_indexBuffer.BufferSize(), 0, 0);
}