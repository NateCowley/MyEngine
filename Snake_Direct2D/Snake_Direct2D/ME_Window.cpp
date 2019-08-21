#include "ME_Window.h"

std::vector<WindowListener*> ME_Window::listeners;

ME_Window::ME_Window()
{

}

ME_Window::~ME_Window()
{

}

bool ME_Window::Init(HINSTANCE hInstance, int nCmdShow, int width, int height, std::string title)
{
	if (beenInitialized)
	{
		return true;
	}

	if (hInstance == NULL || width <= 0 || height <= 0)
	{
		return false;
	}

	this->hInstance = hInstance;
	this->nCmdShow = nCmdShow;
	clientWidth = width;
	clientHeight = height;
	this->windowTitle = (LPCSTR)title.c_str();

	if (!InitWindow())
	{
		return false;
	}

	beenInitialized = true;

	return true;
}

void ME_Window::ShutDown()
{
	UnregisterClass(windowTitle, hInstance);

	for (int i = 0; i > listeners.size(); i++)
	{
		if (listeners[i])
		{
			delete listeners[i];
		}
	}

	listeners.clear();
}

HWND ME_Window::getHandle()
{
	return windowHandle;
}

HINSTANCE ME_Window::getHINSTANCE()
{
	return hInstance;
}

WNDCLASSEX ME_Window::getWindowClass()
{
	return windowClass;
}

RECT ME_Window::getClientDimensions()
{
	return screenRect;
}

int ME_Window::getClientWidth()
{
	return clientWidth;
}

int ME_Window::getClientHeight()
{
	return clientHeight;
}

int ME_Window::getWindowWidth()
{
	// TODO: get size of full window, includeing outline/toolbar
	return clientWidth;
}

int ME_Window::getWindowHeight()
{
	// TODO: get size of full window, includeing outline/toolbar
	return clientWidth;
}

LPCSTR ME_Window::getWindowTitle()
{
	return windowTitle;
}

void ME_Window::addWindowListener(WindowListener* listener)
{
	for (int i = 0; i < listeners.size(); i++)
	{
		if (listener == listeners[i])
		{
			return;
		}
	}

	listeners.push_back(listener);
}

void ME_Window::removeWindowListener(WindowListener* listener)
{
	for (int i = 0; i < listeners.size(); i++)
	{
		if (listener == listeners.at(i))
		{
			listeners.erase(listeners.begin() + i);
			return;
		}
	}
}

void ME_Window::notifyClosing()
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]->Closing();
	}
}

void ME_Window::notifyMinimized()
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]->Minimized();
	}
}

void ME_Window::notifyMaximized()
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]->Maximized();
	}
}

void ME_Window::notifyResized()
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]->Resized();
	}
}

bool ME_Window::InitWindow()
{
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	//windowClass.lpfnWndProc = WindowProc;
	windowClass.lpfnWndProc = ME_WindowProc;
	windowClass.lpszClassName = lpszClassName;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);

	int screenWidth, screenHeight;

	if (!EngineTools::GetDesktopResolution(screenWidth, screenHeight))
	{
		return false;
	}

	RECT wnd = { 0, 0, clientWidth, clientHeight };
	RECT dt;

	GetClientRect(GetDesktopWindow(), &dt);

	AdjustWindowRect(&wnd, WS_OVERLAPPEDWINDOW, FALSE);

	int w = (wnd.right - wnd.left);
	int h = (wnd.bottom - wnd.top);
	int x = ((dt.right - dt.left) - w) / 2;
	int y = ((dt.bottom - dt.top) - h) / 2;

	windowHandle = CreateWindow(lpszClassName, windowTitle, WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, hInstance, NULL);

	if (!windowHandle)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK ME_Window::ME_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		// call Stop to break out of the loop that was started in Start()
		notifyClosing();
		DestroyWindow(hwnd);
		return 0;
	}
	case WM_DESTROY:
	{
		notifyClosing();
		PostQuitMessage(0);
		return 0;
	}
	case WM_SIZING:
	{
		notifyResized();
		return 0;
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}