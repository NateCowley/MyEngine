#include "Snake.h"

Snake::Snake() : DrawableObject(0, 0, 50, 50, color)
{
	
}

Snake::Snake(int x, int y, int w, int h) : DrawableObject(x, y, w, h, color)
{
	xDirection = 0 * width;
	yDirection = 0;

	tempX = 0;
	tempY = 0;

	Input::getInstance()->addKeyboardListener(this);
	Input::getInstance()->addMouseListener(this);

	bits.push_back(new SnakeBit(x, y));

	ac = new AudioComponent((TCHAR*)"A:\\downloads\\Def Leppard - Hysteria.wav");
	ap.setAudioComponent(ac);
	ap.playAt(60);
	ap.setPlayLength(2);
}

Snake::~Snake()
{
	for (int x = 0; x < bits.size(); x++)
	{
		delete bits[x];
	}

	bits.clear();

	if (ac)
	{
		delete ac;
	}

	Input::getInstance()->removeKeyboardListener(this);
	Input::getInstance()->removeMouseListener(this);
}

void Snake::keyDown(int key)
{
	int tempXDirection = 0;
	int tempYDirection = 0;

	switch (key)
	{
	case DIK_W:
	{
		tempXDirection = 0;
		tempYDirection = -1;
		break;
	}
	case DIK_A:
	{
		tempXDirection = -1;
		tempYDirection = 0;
		break;
	}
	case DIK_S:
	{
		tempXDirection = 0;
		tempYDirection = 1;
		break;
	}
	case DIK_D:
	{
		tempXDirection = 1;
		tempYDirection = 0;
		break;
	}
	case DIK_ESCAPE:
	{
		MessageBox(NULL, (std::to_string(xPos) + " " + std::to_string(yPos)).c_str(), "", MB_OK);
		break;
	}
	case DIK_SPACE:
	{
		ap.startPlaying();
		break;
	}
	}

	if (bits.size() > 1)
	{
		if (bits[0]->xLocation + (tempXDirection * width) != bits[1]->xLocation &&
			bits[0]->yLocation + (tempYDirection * height) != bits[1]->yLocation)
		{
			xDirection = tempXDirection;
			yDirection = tempYDirection;
		}
	}
	else
	{
		xDirection = tempXDirection;
		yDirection = tempYDirection;
	}
}

void Snake::keyUp(int key)
{

}

void Snake::tick(long deltaTime)
{
	xPos += xDirection * width;
	yPos += yDirection * height;

	for (int x = bits.size() - 1; x > 0; x--)
	{
		bits[x]->move(bits[x - 1]->xLocation, bits[x - 1]->yLocation);
	}

	bits[0]->move(xPos, yPos);

	if (isColliding(xPos, yPos))
	{
		// find colliding bit, and chop off everything at that point and later
		for (int x = 1; x < bits.size(); x++)
		{
			if (bits[x]->xLocation == xPos && bits[x]->yLocation == yPos)
			{
				chopOffAt(x);
			}
		}
	}
}

void Snake::draw(Graphics* g)
{
	for (int x = 0; x < bits.size(); x++)
	{
		g->DrawFillRect(bits[x]->xLocation, bits[x]->yLocation, width, height, color);
		g->DrawRect(bits[x]->xLocation, bits[x]->yLocation, width, height, Color::PINK, 1.0f);
	}
}

void Snake::addSnakeBit()
{
	bits.push_back(new SnakeBit(bits[bits.size() - 1]->xLocation, bits[bits.size() - 1]->yLocation));
}

bool Snake::isColliding(int xPos, int yPos)
{
	for (int x = 0; x < bits.size(); x++)
	{
		if (bits[x]->xLocation == xPos && bits[x]->yLocation == yPos)
		{
			return true;
		}
	}

	return false;
}

Snake::SnakeBit::SnakeBit(int x, int y)
{
	xLocation = x;
	yLocation = y;
}

void Snake::SnakeBit::move(int x, int y)
{
	xLocation = x;
	yLocation = y;
}

void Snake::chopOffAt(int index)
{
	while (bits.size() > index - 1)
	{
		bits.erase(bits.end() - 1);
	}
}

void Snake::mouseDown(int x)
{
	xDirection = -1;
}

void Snake::mouseUp(int x)
{
	xDirection = 1;
}