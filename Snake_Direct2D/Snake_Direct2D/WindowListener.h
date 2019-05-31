#pragma once

/// <summary>
/// The WindowListener interface is used to detect changes 
/// to the primary window such as resizing, minimizing, 
/// maximizing, etc.
/// </summary>
class WindowListener
{
	/// <summary>
	/// Signal the listeners that the window has been resized and 
	/// send the new width and height to the listeners.
	/// </summary>
	virtual void Resized(int newWidth, int newHeight) = 0;

	/// <summary>
	/// Signal the listeners that the window has been minimized.
	/// </summary>
	virtual void Minimized() = 0;

	/// <summary>
	/// Signal the listeners that the window has been maximized.
	/// </summary>
	virtual void Maximized() = 0;
};