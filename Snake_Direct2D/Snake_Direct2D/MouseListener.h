#pragma once

// MouseListener interface
class MouseListener
{
public:
	virtual void mouseDown(int x) = 0;
	virtual void mouseUp(int x) = 0;
};