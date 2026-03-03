#pragma once
#include "..\\ErrorLogger.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#include <wrl/client.h>
#include <vector>

class DXAdapterData
{
public:
	DXAdapterData(IDXGIAdapter* pAdapter);

	IDXGIAdapter* m_adapter = nullptr;
	DXGI_ADAPTER_DESC m_adapterDesc;
};

class DXAdapterReader
{
public:
	static std::vector<DXAdapterData> GetAdapters();

private:
	static std::vector<DXAdapterData> m_adapters;
};