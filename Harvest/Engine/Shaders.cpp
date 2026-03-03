#include "Shaders.h"

//VERTEX SHADER CLASS

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->m_shaderBuffer.GetAddressOf());
	
	if (FAILED(hr))
	{
		std::wstring errMsg = L"Failed to load vertex shader.";
		errMsg += shaderPath;
		ErrorLogger::Log(hr, errMsg);

		return false;
	}

	hr = device->CreateVertexShader(this->m_shaderBuffer->GetBufferPointer(), this->m_shaderBuffer->GetBufferSize(), NULL, this->m_shader.GetAddressOf());

	if (FAILED(hr))
	{
		std::wstring errMsg = L"Failed to create vertex shader.";
		errMsg += shaderPath;
		ErrorLogger::Log(hr, errMsg);

		return false;
	}

	OutputDebugStringA("DX_INFO: Vertex Shader creation succeeded.\n");

	hr = device->CreateInputLayout(
		layoutDesc,
		numElements,
		this->m_shaderBuffer->GetBufferPointer(),
		this->m_shaderBuffer->GetBufferSize(),
		this->m_inputLayout.GetAddressOf()
	);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "DX_ERROR: ID3D11InputLayout creation failed.");
		return false;
	}

	OutputDebugStringA("DX_INFO: ID3D11InputLayout creation succeeded.\n");

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
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), this->m_shaderBuffer.GetAddressOf());
	
	if (FAILED(hr))
	{
		std::wstring errMsg = L"Failed to load pixel shader.";
		errMsg += shaderPath;
		ErrorLogger::Log(hr, errMsg);

		return false;
	}

	hr = device->CreatePixelShader(this->m_shaderBuffer.Get()->GetBufferPointer(), this->m_shaderBuffer.Get()->GetBufferSize(), NULL, this->m_shader.GetAddressOf());

	if (FAILED(hr))
	{
		std::wstring errMsg = L"Failed to create pixel shader.";
		errMsg += shaderPath;
		ErrorLogger::Log(hr, errMsg);

		return false;
	}

	OutputDebugStringA("DX_INFO: Pixel Shader creation succeeded.\n");
	
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