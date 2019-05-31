#pragma once

#include <dinput.h>

/// <summary>
/// The KeyboardListener interface is used to listen to keyboard events.
/// </summary>
class KeyboardListener
{
public:
	/// <summary>
	/// Send the keyDown event with associated key code
	/// </summary>
	virtual void keyDown(int key) = 0;

	/// <summar>
	/// Send the keyUp event with associated key code
	/// </summary>
	virtual void keyUp(int key) = 0;
};