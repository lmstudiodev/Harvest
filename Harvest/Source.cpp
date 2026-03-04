#include "Harvest.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{

		HRESULT hr = CoInitialize(NULL);
		COM_ERROR_IF_FAILED(hr, "HARVEST_ERROR: Failed to call CoInitialize..");
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return -1;
	}
	
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