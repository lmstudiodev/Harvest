#include "Harvest.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	HRESULT hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to call CoInitialize.");
		OutputDebugStringA("HARVEST_ERROR: Failed to call CoInitialize..\n");
		return -1;
	}

	OutputDebugStringA("HARVEST_INFO: CoInitialize called successfully\n");
	
	Harvest harvest;

	if (harvest.Initialize(hInstance, "HARVEST D3D11 ENGINE", "harvestwndclass", 1920, 1080))
	{
		while (harvest.ProcessMessages() == true)
		{
			harvest.Update();
			harvest.RenderFrame();
		}
	}

	harvest.ShutDown();

	return 0;
}