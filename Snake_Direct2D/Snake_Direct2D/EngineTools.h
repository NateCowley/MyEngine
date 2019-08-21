#pragma once

#include <Windows.h>
#include <string>
#include <atlstr.h>
#include "ME_Window.h"

/// <summary>
/// Used to expedite deleting a single object.
/// </summary>
#define SAFE_DELETE(x) if(x) delete x

/// <summary>
/// Used to expedite deleting an array.
/// </summary>
#define SAFE_DELETE_ARRAY(x) if(x) delete[] x

/// <summary>
/// Used to expedite releasing a single object 
/// that can be released (for example, WIC components).
/// </summary>
#define SAFE_RELEASE(x) if(x) x->Release()

/// <summary>
/// A listing of generic tools that can be used to get 
/// system information and quickly perform other tasks.
/// </summary>
namespace EngineTools
{
	/// <summary>
	/// Retrieves the current resolution of the primary desktop.
	/// </summary>
	bool GetDesktopResolution(int& width, int& height);

	/// <smmary>
	/// Retrieves the current position of the mouse on the screen.
	/// </summary>
	bool GetMouseScreenPos(int& x, int& y, HWND hWnd = GetActiveWindow());

	/// <summary>
	/// Retrieves the resolution of the currently active window.
	/// </summary>
	bool GetClientResolution(int& width, int& height, HWND hWnd = GetActiveWindow());

	/// <summary>
	/// Retrieves the current position of the top left corner of the active window.
	/// </summary>
	bool GetClientPosition(int& x, int& y, HWND hWnd = GetActiveWindow());

	/// <summary>
	/// Retrieve the error message that was last posted, in string format.
	/// </summary>
	std::string GetLastErrorAsString();

	/// <summary>
	/// Immediately create a message box with the last error message that was posted.
	/// </summary>
	void MsgBoxLastErrorMessage();

	/// <summary>
	/// Send the WM_CLOSE signal to the currently active window, so that 
	/// the engine can begin the shutting down process.
	/// </summary>
	void GlobalShutDown();

	/// <summary>
	/// Converts a string to TCHAR*
	/// </summary>
	TCHAR* strToTCHAR(std::string str);

	/// <summary>
	/// Converts a char array to TCHAR*
	/// </summary>
	TCHAR* charToTCHAR(char* chrs);

	/// <summary>
	/// Converts a string to LPCWSTR
	/// </summary>
	LPCWSTR strToLPCWSTR(std::string str);

	/// <summary>
	/// Converts a char array to LPCWSTR
	/// </summary>
	LPCWSTR charToLPCWSTR(char* chrs);
}