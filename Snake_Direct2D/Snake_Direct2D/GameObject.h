#pragma once

class GameObject
{
public:
	double xPos = 0, yPos = 0;
	double width, height;

	GameObject();
	GameObject(int x, int y);
	GameObject(int x, int y, int w, int h);
	~GameObject();

	virtual void tick(long deltaTime);
};