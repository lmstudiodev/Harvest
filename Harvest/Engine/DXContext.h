#pragma once
#include "DXAdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "BufferFactory.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "..\\Timer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <format>

//Episodio 38

using namespace DirectX::DX11;

class DXContext
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	void Release();

public:
	Camera m_camera;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeDepthStencil();
	bool InitializeScene();
	void InitializeImGui(HWND hwnd);

	bool CreateDeviceAndSwapChain(HWND hwnd, std::vector<DXAdapterData> adapters);
	bool CreateRenderTargetView();
	bool CreateSamplerState();
	bool CreateWICTexture(std::wstring path);
	bool CreateVertexBuffer();
	bool CreateIndexBuffer();

	void Draw();
	void DrawTextString(std::wstring msg);
	void DrawImGuiAdapterInfoAndFPS();
	void BeginFrame();
	void EndFrame();

	void UpdateVertexConstantBuffer(float xOffset, float yOffset, float zOffset);

	std::string CalculateFPS();

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rendertargetView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizeState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

	BufferFactory<Vertex> m_vertexBuffer;
	BufferFactory<DWORD> m_indexBuffer;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshader;

	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	std::unique_ptr<SpriteBatch> m_spriteBatch;
	std::unique_ptr<SpriteFont> m_spriteFont;

	std::vector<DXAdapterData> m_adapters;

	int m_windowWidth = 0;
	int m_windowHeight = 0;

	Timer m_FPSTimer;

	bool m_vsync_on = false;

	static float alpha;
};