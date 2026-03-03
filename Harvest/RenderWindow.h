#pragma once
#include "ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hIstance, std::string windows_title, std::string windows_class, int width, int height);
	bool ProcessMessages();
	HWND GetHWND() const;

	~RenderWindow();

private:
	void RegisterWindowClass();

private:
	HWND m_windowHandle = NULL;
	HINSTANCE m_hInstance = NULL;
	std::string m_windows_title = "";
	std::wstring m_windows_title_wide = L"";
	std::string m_windows_class = "";
	std::wstring m_windows_class_wide = L"";

	int m_width = 0;
	int m_height = 0;
};