#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#include "Graphics.h"
#include "Input.h"
#include "Audio.h"
#include "EngineTools.h"
#include "Game.h"
#include "WindowListener.h"

/// <summary>
/// The Window class. Creates the window and sends commands to WindowListeners
/// </summary>
class ME_Window
{
public:
	ME_Window();
	~ME_Window();

	/// <summary>
	/// Creates the window, graphics, and input objects. Must be called before 
	/// prior to anything else. Cannot be called multiple times.
	/// </summary>
	bool Init(HINSTANCE hInstance, int nCmdShow, int width, int height, std::string title = "ME_Window");

	/// <summary>
	/// Shuts down and cleans up the window, graphics, and input objects. 
	/// The function Stop() must be called before Shutdown() in order to avoid 
	/// accidental NullPointerExceptions.
	/// </summary>
	void ShutDown();

	/// <summary>
	/// Retrieves the HWND of the current window.
	/// </summary>
	HWND getHandle();

	/// <summary>
	/// Retrieves the HINSTANCE of the current window.
	/// </summary>
	HINSTANCE getHINSTANCE();

	/// <summary>
	/// Retrieves the WNDCLASSEX of the current window.
	/// </summary>
	WNDCLASSEX getWindowClass();

	/// <summary>
	/// Retrieves the current dimensions of the client RECT
	/// </summary>
	RECT getClientDimensions();

	/// <summary>
	/// Retrieves the width of the client.
	/// </summary>
	int getClientWidth();

	/// <summary>
	/// Retrieves the height of the client.
	/// </summary>
	int getClientHeight();

	/// <summary>
	/// Retrieves the width of the window.
	/// </summary>
	int getWindowWidth();

	/// <summary>
	/// Retrieves the height of the window.
	/// </summary>
	int getWindowHeight();

	/// <summary>
	/// Retrieves the title of the window
	/// </summary>
	LPCSTR getWindowTitle();

	void addWindowListener(WindowListener* listener);

	void removeWindowListener(WindowListener* listener);

private:

	HWND windowHandle;
	HINSTANCE hInstance;
	WNDCLASSEX windowClass;
	int nCmdShow;
	LPCSTR windowTitle;

	static std::vector<WindowListener*> listeners;

	static void notifyClosing();
	static void notifyMinimized();
	static void notifyMaximized();
	static void notifyResized();

	const char* lpszClassName = "MainWindow";

	RECT screenRect;
	int clientWidth, clientHeight;
	int windowWidth, windowHeight;
	bool beenInitialized = false;
	bool shuttingDown;

	/// <summary>
	/// Create the win32 window that the engine will run in.
	/// </summary>
	bool InitWindow();

	static LRESULT CALLBACK ME_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};