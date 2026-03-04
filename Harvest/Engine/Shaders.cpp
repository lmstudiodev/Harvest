#include "Shaders.h"

//VERTEX SHADER CLASS

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	try
	{
		HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->m_shaderBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to load vertex shader.");

		hr = device->CreateVertexShader(this->m_shaderBuffer->GetBufferPointer(), this->m_shaderBuffer->GetBufferSize(), NULL, this->m_shader.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to create vertex shader.");

		hr = device->CreateInputLayout(
			layoutDesc,
			numElements,
			this->m_shaderBuffer->GetBufferPointer(),
			this->m_shaderBuffer->GetBufferSize(),
			this->m_inputLayout.GetAddressOf()
		);

		COM_ERROR_IF_FAILED(hr, "DX_ERROR: ID3D11InputLayout creation failed.");
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->m_shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return this->m_shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->m_inputLayout.Get();
}

void VertexShader::ShutDown()
{
	m_shader.Reset();
	m_shaderBuffer.Reset();
	m_inputLayout.Reset();
}

//PIXEL SHADER CLASS

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath)
{
	try
	{
		HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->m_shaderBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to load pixel shader.");

		hr = device->CreatePixelShader(this->m_shaderBuffer.Get()->GetBufferPointer(), this->m_shaderBuffer.Get()->GetBufferSize(), NULL, this->m_shader.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to create pixel shader.");

	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->m_shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->m_shaderBuffer.Get();
}

void PixelShader::ShutDown()
{
	m_shader.Reset();
	m_shaderBuffer.Reset();
}