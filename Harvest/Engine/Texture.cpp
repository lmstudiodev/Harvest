#include "Texture.h"
#include "..\\ErrorLogger.h"

Texture::Texture(ID3D11Device* device, const Color& color, aiTextureType type)
{
	this->Initialize1x1ColorTexture(device, color, type);
}

Texture::Texture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->InitializeColorTexture(device, colorData, width, height, type);
}

aiTextureType Texture::GetType()
{
	return this->m_type;
}

ID3D11ShaderResourceView* Texture::GetTextureResourceView()
{
	return this->m_textureView.Get();
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
	return this->m_textureView.GetAddressOf();
}

void Texture::Initialize1x1ColorTexture(ID3D11Device* device, const Color& colorData, aiTextureType type)
{
	InitializeColorTexture(device, &colorData, 1, 1, type);
}

void Texture::InitializeColorTexture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->m_type = type;

	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

	ID3D11Texture2D* p2DTexture = nullptr;

	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = colorData;
	initialData.SysMemPitch = width * sizeof(Color);

	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
	COM_ERROR_IF_FAILED(hr, "[D3D11] Failed to initialize texture from color data");

	m_texture = static_cast<ID3D11Texture2D*>(p2DTexture);

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_textureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "[D3D11] Failed to create shared resource view from texture generated from color data");
}
