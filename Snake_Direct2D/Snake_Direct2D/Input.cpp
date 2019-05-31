#include "Input.h"

Input* Input::instance = 0;

Input* Input::getInstance()
{
	if (!instance)
	{
		instance = new Input();
	}

	return instance;
}

Input::Input()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;

	// set all of the keystates to false
	for (int x = 0; x < 256; x++)
	{
		m_keyState[x] = false;
	}

	// set all of the mouse button states to false
	for (int x = 0; x < 4; x++)
	{
		m_mouseButtonState[x] = false;
	}
}

Input::~Input()
{

}

void Input::Shutdown()
{
	// release the mouse
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// release the keyboard
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// release the main interface to direct input
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	for (int x = 0; x < mouseListeners.size(); x++)
	{
		if (mouseListeners[x])
		{
			delete mouseListeners[x];
		}
	}

	mouseListeners.clear();

	for (int x = 0; x < keyboardListeners.size(); x++)
	{
		if (keyboardListeners[x])
		{
			delete keyboardListeners[x];
		}
	}

	keyboardListeners.clear();
}

bool Input::Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	getInstance();

	HRESULT result;

	// store the screen size which will be used for positioning the mouse cursor
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// initialize the location of the mouse on the screen
	m_mouseX = 0;
	m_mouseY = 0;

	// initialize the main direct input interface
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// initialize the direct input interface for the keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// set the data format. In this case, since it is a keyboard, we can use the predefined data format
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// set the cooperative level of the keyboard to not thare with other programs
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// now acquire the keyboard
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// initialize the direct input interface for the mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// set the data format for the mouse using the pre-defined mouse data format
	result = m_mouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		return false;
	}

	// set the cooperative level of the mouse to share with other programs
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// acquire the mouse
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

// The Frame function for the InputClass will read the current state of the devices 
// into state buffers we setup. After the state of each device is read it then 
// processes the changes
bool Input::Frame()
{
	if (!instance)
	{
		return false;
	}

	bool result;

	// read the current state of the keyboard
	result = ReadKeyBoard();
	if (!result)
	{
		return false;
	}

	// read the current state of the mouse
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// process the changes in the mouse and keyboard
	ProcessInput();

	return true;
}

bool Input::ReadKeyBoard()
{
	HRESULT result;

	// read the keyboard device
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// if the keyboard lost focus or was not acquired then try to get control back
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;

	// read the mosue device
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// if the mouse lost focus or was not acquired then try to get control back
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Input::ProcessInput()
{
	// update the location of the mouse cursor based on the change of the mouse location during the frame
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// ensure the mouse location doesn't exceed the screen width or height
	if (m_mouseX < 0)
	{
		m_mouseX = 0;
	}

	if (m_mouseY < 0)
	{
		m_mouseY = 0;
	}

	if (m_mouseX > m_screenWidth)
	{
		m_mouseX = m_screenWidth;
	}

	if (m_mouseY > m_screenHeight)
	{
		m_mouseY = m_screenHeight;
	}

	// check mouse states
	for (int x = 0; x < 4; x++)
	{
		if (m_mouseState.rgbButtons[x] != m_mouseButtonState[x])
		{
			if (m_mouseState.rgbButtons[x] && !m_mouseButtonState[x])
			{
				m_mouseButtonState[x] = true;

				for (int i = 0; i < mouseListeners.size(); i++)
				{
					mouseListeners.at(i)->mouseDown(x);
				}
			}
			else if (!m_mouseState.rgbButtons[x] && m_mouseButtonState[x])
			{
				m_mouseButtonState[x] = false;

				for (int i = 0; i < mouseListeners.size(); i++)
				{
					mouseListeners.at(i)->mouseUp(x);
				}
			}
		}
	}

	for (int x = 0; x < 255; x++)
	{
		// if the keyboardState for the key doesn't match the keystate, fix it
		if (m_keyboardState[x] != m_keyState[x])
		{
			// if the keyboardState says it's down, but the keyState says it's up, fix and send keyDown message
			if (m_keyboardState[x] && !m_keyState[x])
			{
				m_keyState[x] = true;

				// send the keyDown signal to listeners
				for (int i = 0; i < keyboardListeners.size(); i++)
				{
					keyboardListeners.at(i)->keyDown(x);
				}
			}
			// if the keyboardState says it's up, but the keyState says it's down, fix and send keyUp message
			else if (!m_keyboardState[x] && m_keyState[x])
			{
				m_keyState[x] = false;

				// send the keyUp signal to listeners
				for (int i = 0; i < keyboardListeners.size(); i++)
				{
					keyboardListeners.at(i)->keyUp(x);
				}
			}
		}
	}
}

void Input::GetMouseLocation(int& mouseX, int& mouseY)
{
	if (!instance)
	{
		return;
	}

	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

void Input::addMouseListener(MouseListener* ml)
{
	if (!instance)
	{
		return;
	}

	mouseListeners.push_back(ml);
}

void Input::addKeyboardListener(KeyboardListener* kl)
{
	if (!instance)
	{
		return;
	}

	keyboardListeners.push_back(kl);
}

void Input::removeMouseListener(MouseListener* ml)
{
	if (!instance)
	{
		return;
	}

	for (int x = 0; x < mouseListeners.size(); x++)
	{
		if (ml == mouseListeners.at(x))
		{
			mouseListeners.erase(mouseListeners.begin() + x);
			break;
		}
	}
}

void Input::removeKeyboardListener(KeyboardListener* kl)
{
	if (!instance)
	{
		return;
	}

	for (int x = 0; x < keyboardListeners.size(); x++)
	{
		if (kl == keyboardListeners.at(x))
		{
			keyboardListeners.erase(keyboardListeners.begin() + x);
			break;
		}
	}
}