#pragma once

/// <summary>
/// Used to connect to the Input class and listen for mouse events
/// </summary>
class MouseListener
{
public:
	/// <summary>
	/// The event that is fired by the Input class when the left mouse button is pressed down.
	/// </summary>
	virtual void mouseDown(int x) = 0;

	/// <summary>
	/// The event that is fired by the Input class when the left mouse button is released.
	/// </summary>
	virtual void mouseUp(int x) = 0;
};