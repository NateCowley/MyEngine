#pragma once

#include <vector>
#include <windows.h>
#include <string>

#include "DrawableObject.h"
#include "KeyboardListener.h"
#include "Input.h"
#include "MouseListener.h"
#include "AudioPlayer.h"

class Graphics;

class Snake : public DrawableObject, public KeyboardListener, public MouseListener
{
public:

	Snake();
	Snake(int x, int y, int w, int h);
	~Snake();

	virtual void keyDown(int key);
	virtual void keyUp(int key);

	virtual void tick(long deltaTime);
	virtual void draw(Graphics* g);

	virtual void mouseDown(int x);
	virtual void mouseUp(int x);

	void addSnakeBit();

	bool isColliding(int xPos, int yPos);

private:

	int xDirection, yDirection;
	int tempX, tempY;
	int maxX, maxY;
	D2D1::ColorF color = Color::GREEN;
	AudioComponent* ac;
	AudioPlayer ap;

	struct SnakeBit
	{
		int xLocation = 0, yLocation = 0;

		SnakeBit(int x, int y);

		void move(int x, int y);
	};

	std::vector<SnakeBit*> bits;

	void chopOffAt(int index);
};