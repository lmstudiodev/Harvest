#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__
#include <d3d11.h>
#include "ConstantBufferTypes.h"
#include <wrl/client.h>
#include "..\\ErrorLogger.h"

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	ID3D11DeviceContext* m_deviceContext = nullptr;

public:
	ConstantBuffer() {}

	T data;

	ID3D11Buffer* Get() const
	{
		return m_buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()
	{
		return m_buffer.GetAddressOf();
	}

	void ShutDown()
	{
		if (this->m_buffer != nullptr)
			this->m_buffer.Reset();
	}

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		if (this->m_buffer != nullptr)
			this->m_buffer.Reset();
		
		this->m_deviceContext = deviceContext;

		D3D11_BUFFER_DESC bufferDesc;

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - sizeof(T) % 16));
		bufferDesc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&bufferDesc, 0, m_buffer.GetAddressOf());

		if (FAILED(hr))
		{
			OutputDebugStringA("DX_INFO: Constant buffer initialization failed.\n");
			return false;
		}

		OutputDebugStringA("DX_INFO: Constant buffer initialization success.\n");

		return true;
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		HRESULT hr = this->m_deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(hr))
		{
			OutputDebugStringA("DX_INFO: Constant buffer mapping failed.\n");
			return false;
		}

		CopyMemory(mappedResource.pData, &data, sizeof(T));
		this->m_deviceContext->Unmap(m_buffer.Get(), 0);

		return true;
	}
};

#endif // !ConstantBuffer_h__