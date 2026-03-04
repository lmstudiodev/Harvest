#ifndef BufferFactory_h__
#define BufferFactory_h__
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<class T>
class BufferFactory
{
private:
	BufferFactory(const BufferFactory<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	std::unique_ptr<UINT> m_stride;
	UINT m_bufferSize = 0;

public:
	BufferFactory() {}

	ID3D11Buffer* Get() const
	{
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return m_buffer.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return m_bufferSize;
	}

	const UINT Stride() const
	{
		return *this->m_stride.get();
	}

	const UINT* StridePointer() const
	{
		return this->m_stride.get();
	}

	void ShutDown()
	{
		if (this->m_buffer != nullptr)
			this->m_buffer.Reset();
	}

	HRESULT Initialize(ID3D11Device* device, T* data, UINT numVertices, bool isVertexBuffer = true)
	{
		if (this->m_buffer != nullptr)
			this->m_buffer.Reset();
		
		this->m_bufferSize = numVertices;

		if (isVertexBuffer)
		{
			if(this->m_stride.get() == nullptr)
				this->m_stride = std::make_unique<UINT>(sizeof(T));
		}

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(T) * numVertices;
		bufferDesc.BindFlags = isVertexBuffer ? D3D11_BIND_VERTEX_BUFFER : D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

		vertexBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&bufferDesc, &vertexBufferData, this->m_buffer.GetAddressOf());

		return hr;
	}
};

#endif // !BufferFactory_h__

