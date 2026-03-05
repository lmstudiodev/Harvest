#ifndef BufferFactory_h__
#define BufferFactory_h__
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>

template<class T>
class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_stride = sizeof(T);
	UINT m_vertexCount = 0;

public:
	VertexBuffer() {}

	VertexBuffer(const VertexBuffer<T>& rhs)
	{
		this->m_buffer = rhs.m_buffer;
		this->m_vertexCount = rhs.m_vertexCount;
		this->m_stride = rhs.m_stride;
	}

	VertexBuffer<T>& operator=(const VertexBuffer<T>& a)
	{
		this->m_buffer = a.m_buffer;
		this->m_vertexCount = a.m_vertexCount;
		this->m_stride = a.m_stride;
		return *this;
	}

	ID3D11Buffer* Get()const
	{
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()const
	{
		return m_buffer.GetAddressOf();
	}

	UINT VertexCount() const
	{
		return this->bufferSize;
	}

	const UINT Stride() const
	{
		return m_stride;
	}

	const UINT* StridePointer() const
	{
		return &this->m_stride;
	}

	HRESULT Initialize(ID3D11Device* device, T* data, UINT vertexCount)
	{
		if (m_buffer.Get() != nullptr)
			m_buffer.Reset();

		this->m_vertexCount = vertexCount;
		//if (this->m_stride.get() == nullptr)
		//	this->m_stride = std::make_shared<UINT>(sizeof(T));

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = m_stride * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->m_buffer.GetAddressOf());
		return hr;
	}
};

class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_indexCount = 0;
public:
	IndexBuffer() {}

	ID3D11Buffer* Get()const
	{
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()const
	{
		return m_buffer.GetAddressOf();
	}

	UINT IndexCount() const
	{
		return this->m_indexCount;
	}

	HRESULT Initialize(ID3D11Device* device, DWORD* data, UINT indexCount)
	{
		if (m_buffer.Get() != nullptr)
			m_buffer.Reset();

		this->m_indexCount = indexCount;
		//Load Index Data
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData;
		indexBufferData.pSysMem = data;
		HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, m_buffer.GetAddressOf());
		return hr;
	}
};

#endif // !BufferFactory_h__

