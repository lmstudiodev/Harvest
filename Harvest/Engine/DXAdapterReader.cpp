#include "DXAdapterReader.h"

std::vector<DXAdapterData> DXAdapterReader::m_adapters;

std::vector<DXAdapterData> DXAdapterReader::GetAdapters()
{
	try
	{
		if (m_adapters.size() > 0)
			return m_adapters;

		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "DX_ERROR: Failed to create DXGIFactory for enumeratings adapters.");

		IDXGIAdapter* pAdapter;
		UINT index = 0;

		while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
		{
			m_adapters.push_back(DXAdapterData(pAdapter));
			index += 1;
		}
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		exit(-1);
	}

	return m_adapters;
}

DXAdapterData::DXAdapterData(IDXGIAdapter* pAdapter)
{
	this->m_adapter = pAdapter;

	HRESULT hr = pAdapter->GetDesc(&this->m_adapterDesc);

	if (FAILED(hr))
	{
		OutputDebugStringA("DX_ERROR: Failed to get description for IDXGIAdapter.\n");
		ErrorLogger::Log(hr, "Failed to get description for IDXGIAdapter.");
	}
}
