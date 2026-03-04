#include "Harvest.h"

bool Harvest::Initialize(HINSTANCE hIstance, std::string windows_title, std::string windows_class, int width, int height)
{
	m_timer.Start();
	
	if (!this->render_window.Initialize(this, hIstance, windows_title, windows_class, width, height))
		return false;

	if (!dxContext.Initialize(this->render_window.GetHWND(), width, height))
		return false;

	return true;
}

bool Harvest::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Harvest::Update()
{
	float dt = m_timer.GetMillisecondsElapsed();
	m_timer.Restart();
	
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char c = keyboard.ReadChar();
	}

	while (!keyboard.KeyboardEventBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();

		unsigned char keycode = kbe.GetKeycode();
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();

		if (mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::MouseEventType::RAW_MOVE)
			{
				this->dxContext.m_camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0.0f);
			}
		}
	}

	this->dxContext.model.AdjustRotation(0.0f, 0.001f * dt, 0.0f);

	const float cameraSpeed = 0.003f;

	if (keyboard.KeyIsPressed('W'))
	{
		this->dxContext.m_camera.AdjustPosition(this->dxContext.m_camera.GetForwardVector() * cameraSpeed * dt);
	}

	if (keyboard.KeyIsPressed('S'))
	{
		this->dxContext.m_camera.AdjustPosition(this->dxContext.m_camera.GetBackwardVector() * cameraSpeed * dt);
	}

	if (keyboard.KeyIsPressed('A'))
	{
		this->dxContext.m_camera.AdjustPosition(this->dxContext.m_camera.GetLeftVector() * cameraSpeed * dt);
	}

	if (keyboard.KeyIsPressed('D'))
	{
		this->dxContext.m_camera.AdjustPosition(this->dxContext.m_camera.GetRightVector() * cameraSpeed * dt);
	}

	if (keyboard.KeyIsPressed('D'))
	{
		this->dxContext.m_camera.AdjustPosition(this->dxContext.m_camera.GetRightVector() * cameraSpeed * dt);
	}

	if (keyboard.KeyIsPressed(VK_SPACE))
	{
		this->dxContext.m_camera.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
	}

	if (keyboard.KeyIsPressed('Z'))
	{
		this->dxContext.m_camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
	}
}

void Harvest::RenderFrame()
{
	dxContext.RenderFrame();
}

void Harvest::ShutDown()
{
	dxContext.Release();
}
