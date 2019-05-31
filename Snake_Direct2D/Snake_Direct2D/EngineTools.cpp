#include "EngineTools.h"

bool EngineTools::GetDesktopResolution(int& width, int& height)
{
	RECT desktop;

	const HWND hDesktop = GetDesktopWindow();

	if (GetWindowRect(hDesktop, &desktop))
	{
		width = desktop.right;
		height = desktop.bottom;
		return true;
	}

	return false;
}

bool EngineTools::GetMouseScreenPos(int& x, int& y, HWND hWnd)
{
	POINT p;

	GetCursorPos(&p);

	if (ScreenToClient(hWnd, &p))
	{
		x = p.x;
		y = p.y;
		return true;
	}

	return false;
}

bool EngineTools::GetClientResolution(int& width, int& height, HWND hWnd)
{
	RECT temp;

	if (!GetClientRect(hWnd, &temp))
	{
		return false;
	}

	width = temp.right;
	height = temp.bottom;

	return true;
}

bool EngineTools::GetClientPosition(int& x, int& y, HWND hWnd)
{
	RECT temp;

	if (!GetClientRect(hWnd, &temp))
	{
		return false;
	}

	x = temp.left;
	y = temp.top;

	return true;
}

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string EngineTools::GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
	{
		return std::string(); //No error message has been recorded
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, 
		errorMessageID, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPSTR)&messageBuffer, 
		0, 
		NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

void EngineTools::MsgBoxLastErrorMessage()
{
	MessageBox(NULL, EngineTools::GetLastErrorAsString().c_str(), "Last Error", MB_OK);
}

void EngineTools::GlobalShutDown()
{
	PostMessage(GetActiveWindow(), WM_CLOSE, 0, 0);
}