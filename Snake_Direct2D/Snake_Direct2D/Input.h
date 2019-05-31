#pragma once

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <vector>
#include <string>

#include "MouseListener.h"
#include "KeyboardListener.h"

static const int MOUSE_LEFT = 0;
static const int MOUSE_RIGHT = 0;

/// <summary>
/// The Input class captures input and relays input information to 
/// the appropriate listeners. It is a singleton.
/// </summary>
class Input
{
public:
	
	/// <summary>
	/// Retrieve the Input instance.
	/// </summary>
	static Input* getInstance();
	~Input();

	/// <summary>
	/// Create the Input object and begin to capture all of the input 
	/// related data.
	/// </summary>
	bool Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	
	/// <summary>
	/// Shutdown the Input object. This method is used to clean up the Input object.
	/// Must be called before destroying the Input object.
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Retrieve all data from captured devices and relay that info to 
	/// the appropriate listeners.
	/// </summary>
	bool Frame();

	/// <summary>
	/// Retrieve the current x and y locations of the mouse pointer on 
	/// the screen.
	/// </summary>
	void GetMouseLocation(int& mouseX, int& mouseY);

	/// <summary>
	/// Adds a mouseListener to the list of MouseListeners.
	/// </summary>
	void addMouseListener(MouseListener* ml);

	/// <summary>
	/// Adds a keyboardListener to the list of KeyboardListeners.
	/// </summary>
	void addKeyboardListener(KeyboardListener* kl);

	/// <summary>
	/// Removes a MouseListener from the list of MouseListeners.
	/// </summary>
	void removeMouseListener(MouseListener* ml);

	/// <summary>
	/// Removes a KeyboardListener from the list of KeyboardListeners.
	/// </summary>
	void removeKeyboardListener(KeyboardListener* kl);

private:

	static Input* instance;
	Input();

	/// <summary>
	/// Retrieves the current state of the keyboard.
	/// </summary>
	bool ReadKeyBoard();

	/// <summary>
	/// Retrieves the current state of the mouse.
	/// </summary>
	bool ReadMouse();

	/// <summary>
	/// Processes the data after ReadKeyBoard and ReadMouse 
	/// have been called. Sends the appropriate signals to 
	/// the appropriate Keyboard and MouseListeners
	/// </summary>
	void ProcessInput();

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	bool m_keyState[256];
	DIMOUSESTATE2 m_mouseState;
	bool m_mouseButtonState[4];

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	std::vector<MouseListener*> mouseListeners;
	std::vector<KeyboardListener*> keyboardListeners;
};