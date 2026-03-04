#include "DXContext.h"

float DXContext::alpha = 1.0f;

bool DXContext::Initialize(HWND hwnd, int width, int height)
{
    this->m_windowWidth = width;
    this->m_windowHeight = height;

    this->m_FPSTimer.Start();
    
    if(!InitializeDirectX(hwnd))
        return false;

    InitializeImGui(hwnd);
}

#pragma region RENDER

void DXContext::RenderFrame()
{
    BeginFrame();
    Draw();

#if defined(_DEBUG)
    DrawImGuiAdapterInfoAndFPS();
#endif

    EndFrame();
}

void DXContext::BeginFrame()
{
    float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    this->m_deviceContext->ClearRenderTargetView(this->m_rendertargetView.Get(), bgColor);
    this->m_deviceContext->ClearDepthStencilView(this->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

    this->m_deviceContext->IASetInputLayout(this->m_vertexShader.GetInputLayout());
    this->m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    this->m_deviceContext->RSSetState(this->m_rasterizeState.Get());
    this->m_deviceContext->OMSetDepthStencilState(this->m_depthStencilState.Get(), 0);
    this->m_deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);

    this->m_deviceContext->PSSetSamplers(0, 1, this->m_samplerState.GetAddressOf());

    this->m_deviceContext->VSSetShader(m_vertexShader.GetShader(), NULL, 0);
    this->m_deviceContext->PSSetShader(m_pixelShader.GetShader(), NULL, 0);
}

void DXContext::Draw()
{
    UINT offset = 0;

    //this->cb_ps_pixelshader.data.alpha = 1.0f;
    //this->cb_ps_pixelshader.ApplyChanges();
    //this->m_deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

    //static float translationOffset[3] = { 0.0f, 0.0f, -1.0f };
    //DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);

    this->model.Draw(m_camera.GetViewMatrix() * m_camera.GetProjectionMatrix());
}

void DXContext::DrawTextString(std::wstring msg)
{
    m_spriteBatch->Begin();

    m_spriteFont->DrawString(m_spriteBatch.get(),
        msg.c_str(),
        DirectX::XMFLOAT2(10.0f, 10.0f),
        DirectX::Colors::White, 0.0f,
        DirectX::XMFLOAT2(0.0f, 0.0f),
        DirectX::XMFLOAT2(1.0f, 1.0f)
    );

    m_spriteBatch->End();
}

void DXContext::DrawImGuiAdapterInfoAndFPS()
{
    auto adapterDesc = m_adapters[0].m_adapterDesc;
    std::wstring _ad(adapterDesc.Description);
    std::string _adapterName(_ad.begin(), _ad.end());
    auto vm = (FLOAT)((adapterDesc.DedicatedVideoMemory / 1024) / 1024) / 1000;
    std::string _videoMemory = std::format("VRam {} GB", std::to_string((UINT)std::ceil(vm)));
    std::string _vsync = std::format("VSync: {}", m_vsync_on ? "ON" : "OFF");
    
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    ImGui::Begin("Debug Info");
    ImGui::Text(_adapterName.c_str());
    ImGui::Text(_videoMemory.c_str());
    ImGui::Text(_vsync.c_str());
    ImGui::SameLine(100.0f);

    if (ImGui::Button(m_vsync_on ? "OFF" : "ON"))
        m_vsync_on = !m_vsync_on;
    
    ImGui::Text(CalculateFPS().c_str());
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("Alpha");
    ImGui::DragFloat("value", &alpha, 0.1f, 0.0f, 1.0f);
    ImGui::End();

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DXContext::EndFrame()
{
    this->m_swapChain->Present(m_vsync_on ? 1 : 0, NULL);
}

#pragma endregion

void DXContext::UpdateVertexConstantBuffer(float xOffset, float yOffset, float zOffset)
{
    DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

    cb_vs_vertexshader.data.mat = world * m_camera.GetViewMatrix() * m_camera.GetProjectionMatrix();
    cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);

    if (!cb_vs_vertexshader.ApplyChanges())
        return;
}

bool DXContext::InitializeDirectX(HWND hwnd)
{        
    std::vector<DXAdapterData> adapters = DXAdapterReader::GetAdapters();

    if (adapters.size() < 1)
    {
        ErrorLogger::Log("DX_ERROR: No DXGI Adapter found.");
        return false;
    }

    if (!CreateDeviceAndSwapChain(hwnd, adapters))
        return false;

    if (!CreateRenderTargetView())
        return false;

    m_spriteBatch = std::make_unique<SpriteBatch>(this->m_deviceContext.Get());
    m_spriteFont = std::make_unique<SpriteFont>(this->m_device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

    if (!CreateSamplerState())
        return false;

    if (!InitializeShaders())
        return false;

    if (!InitializeScene())
        return false;
    
    return true;
}

bool DXContext::InitializeShaders()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    UINT numElements = ARRAYSIZE(layout);
    
    if (!m_vertexShader.Initialize(this->m_device, L"..\\x64\\Debug\\vertexshader.cso", layout, numElements))
        return false;

    if (!m_pixelShader.Initialize(this->m_device, L"..\\x64\\Debug\\pixelshader.cso"))
        return false;
    
    return true;
}

bool DXContext::InitializeDepthStencil()
{
    try
    {
        CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->m_windowWidth, this->m_windowHeight);
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        HRESULT hr = this->m_device->CreateTexture2D(&depthStencilDesc, NULL, this->m_depthStencilBuffer.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Depth Stencil Buffer creation failed.");

        hr = this->m_device->CreateDepthStencilView(this->m_depthStencilBuffer.Get(), NULL, this->m_depthStencilView.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Depth Stencil View creation failed.");

        this->m_deviceContext->OMSetRenderTargets(1, this->m_rendertargetView.GetAddressOf(), this->m_depthStencilView.Get());

        CD3D11_DEPTH_STENCIL_DESC depthStencileStateDesc(D3D11_DEFAULT);
        depthStencileStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        hr = this->m_device->CreateDepthStencilState(&depthStencileStateDesc, this->m_depthStencilState.GetAddressOf());
        COM_ERROR_IF_FAILED(0, "DX_ERROR: Depth Stencil State creation failed.");
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

bool DXContext::InitializeScene()
{
    if (!CreateWICTexture(L"Data\\Textures\\rockwall.jpg"))
        return false;

    if (!cb_vs_vertexshader.Initialize(m_device.Get(), m_deviceContext.Get()))
        return false;

    if (!cb_ps_pixelshader.Initialize(m_device.Get(), m_deviceContext.Get()))
        return false;

    if (!model.Initialize(this->m_device.Get(), this->m_deviceContext.Get(), this->m_texture.Get(), cb_vs_vertexshader))
        return false;

    float aspectRatio = static_cast<float>(this->m_windowWidth) / static_cast<float>(this->m_windowHeight);

    m_camera.SetPosition(0.0f, 0.0f, -2.0f);
    m_camera.SetProjectionValue(90.0f, aspectRatio, 0.1f, 1000.0f);

    return true;
}

void DXContext::InitializeImGui(HWND hwnd)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(this->m_device.Get(), this->m_deviceContext.Get());
    ImGui::StyleColorsDark();
}

bool DXContext::CreateRenderTargetView()
{
    try
    {
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};

        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        HRESULT hr = this->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Get BUFFER failed.");

        hr = this->m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_rendertargetView.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: ID3D11RenderTargetView creation failed.");
        if (!InitializeDepthStencil())
            return false;

        CD3D11_VIEWPORT viewPort(0.0f, 0.0f, static_cast<float>(this->m_windowWidth), static_cast<float>(this->m_windowHeight));

        this->m_deviceContext->RSSetViewports(1, &viewPort);

        CD3D11_RASTERIZER_DESC rastDesc(D3D11_DEFAULT);

        hr = this->m_device->CreateRasterizerState(&rastDesc, this->m_rasterizeState.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Rasterized state creation failed.");

        CD3D11_RASTERIZER_DESC rastDesc_cullFront(D3D11_DEFAULT);
        rastDesc_cullFront.CullMode = D3D11_CULL_FRONT;

        hr = this->m_device->CreateRasterizerState(&rastDesc_cullFront, this->m_rasterizeStateCullFront.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Rasterized state creation failed.");

        D3D11_RENDER_TARGET_BLEND_DESC rtbd = {0};
        rtbd.BlendEnable = true;
        rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
        rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
        rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
        rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
        rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
        rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
        rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

        D3D11_BLEND_DESC blendDesc = { 0 };
        blendDesc.RenderTarget[0] = rtbd;

        hr = this->m_device->CreateBlendState(&blendDesc, this->m_blendState.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Blend state creation failed.");
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

bool DXContext::CreateSamplerState()
{
    try
    {
        CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        HRESULT hr = this->m_device->CreateSamplerState(&samplerDesc, this->m_samplerState.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Sampler State creation failed.");
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

bool DXContext::CreateDeviceAndSwapChain(HWND hwnd, std::vector<DXAdapterData> adapters)
{
    try
    {
        DXGI_SWAP_CHAIN_DESC scd = {0};
        scd.BufferDesc.Width = this->m_windowWidth;
        scd.BufferDesc.Height = this->m_windowHeight;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.BufferCount = 1;
        scd.OutputWindow = hwnd;
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        UINT creationFlags = NULL;

#if defined(_DEBUG)
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        HRESULT hr = D3D11CreateDeviceAndSwapChain(adapters[0].m_adapter,
            D3D_DRIVER_TYPE_UNKNOWN,
            NULL,
            creationFlags,
            NULL,
            0,
            D3D11_SDK_VERSION,
            &scd,
            this->m_swapChain.GetAddressOf(),
            this->m_device.GetAddressOf(),
            NULL,
            this->m_deviceContext.GetAddressOf()
        );

        COM_ERROR_IF_FAILED(hr, "DX_ERROR: ID3D11Device and IDXGISwapChain creation failed.");

        this->m_adapters = adapters;
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

bool DXContext::CreateWICTexture(std::wstring path)
{
    try
    {
        HRESULT hr = DirectX::CreateWICTextureFromFile(this->m_device.Get(), path.c_str(), nullptr, this->m_texture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to create texture.");
    }
    catch (COMException& exception)
    {
        ErrorLogger::Log(exception);
        return false;
    }

    return true;
}

void DXContext::Release()
{
    cb_vs_vertexshader.ShutDown();

    m_vertexShader.ShutDown();
    m_pixelShader.ShutDown();
    
    Microsoft::WRL::ComPtr<ID3D11Debug> debug;

    if (SUCCEEDED(m_device->QueryInterface(IID_PPV_ARGS(&debug))))
    {
        debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    }
}

std::string DXContext::CalculateFPS()
{
    static int fpsCounter = 0;
    static std::string fpsString = "FPS: 0";

    fpsCounter += 1;

    if (this->m_FPSTimer.GetMillisecondsElapsed() > 1000.0)
    {
        fpsString = "FPS: " + std::to_string(fpsCounter);
        fpsCounter = 0;
        this->m_FPSTimer.Restart();
    }

    return fpsString;
}
