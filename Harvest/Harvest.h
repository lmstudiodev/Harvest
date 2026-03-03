#pragma once
#include "WindowContainer.h"
#include "Timer.h"

class Harvest : WindowContainer
{
public:
	bool Initialize(HINSTANCE hIstance, std::string windows_title, std::string windows_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
	void ShutDown();

private:
	Timer m_timer;
};