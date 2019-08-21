#pragma once

/// <summary>
/// The WindowListener interface is used to detect changes 
/// to the primary window such as resizing, minimizing, 
/// maximizing, etc.
/// </summary>
class WindowListener
{
public:
	/// <summary>
	/// Signal the listeners that the window has been resized and 
	/// send the new width and height to the listeners.
	/// </summary>
	virtual void Resized() = 0;

	/// <summary>
	/// Signal the listeners that the window has been minimized.
	/// </summary>
	virtual void Minimized() = 0;

	/// <summary>
	/// Signal the listeners that the window has been maximized.
	/// </summary>
	virtual void Maximized() = 0;

	/// <summary>
	/// Signal the listeners that the window is being closed
	/// <para></para> (either through the X button being pressed 
	/// or the kill signal being sent).
	/// </summary>
	virtual void Closing() = 0;
};

namespace WindowAction
{
	const int RESIZED = 0;
	const int MINIMIZED = 1;
	const int MAXIMIZED = 2;
	const int CLOSING = 3;
}