#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hIstance, std::string windows_title, std::string windows_class, int width, int height)
{
	this->m_hInstance = hIstance;
	this->m_width = width;
	this->m_height = height;
	this->m_windows_title = windows_title;
	this->m_windows_title_wide = StringConverter::StringToWide(this->m_windows_title);
	this->m_windows_class = windows_class;
	this->m_windows_class_wide = StringConverter::StringToWide(this->m_windows_class);
	
	this->RegisterWindowClass();

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - (this->m_width / 2);
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - (this->m_height / 2);

	RECT wr;
	wr.left = centerScreenX;
	wr.top = centerScreenY;
	wr.right = wr.left + this->m_width;
	wr.bottom = wr.top + this->m_height;

	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	this->m_windowHandle = CreateWindowEx(0,
		this->m_windows_class_wide.c_str(),
		this->m_windows_title_wide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		this->m_hInstance,
		pWindowContainer);

	if (this->m_windowHandle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "CreateWindowEx failed for Window: " + this->m_windows_title);
		return false;
	}

	ShowWindow(this->m_windowHandle, SW_SHOW);
	SetForegroundWindow(this->m_windowHandle);
	SetFocus(this->m_windowHandle);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, this->m_windowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->m_windowHandle))
		{
			this->m_windowHandle = NULL;
			UnregisterClass(this->m_windows_class_wide.c_str(), this->m_hInstance);
			return false;
		}
	}
	
	return true;
}

HWND RenderWindow::GetHWND() const
{
	return this->m_windowHandle;
}

RenderWindow::~RenderWindow()
{
	if (this->m_windowHandle != NULL)
	{
		UnregisterClass(this->m_windows_class_wide.c_str(), this->m_hInstance);
		DestroyWindow(m_windowHandle);
	}
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return 0;
	}
	default:
	{
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		
		if (pWindow == nullptr)
		{
			ErrorLogger::Log("Critical error: pointer to window container is NULL during WM_NCCREATE.\n");
			exit(-1);
		}

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->m_hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->m_windows_class_wide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
}
