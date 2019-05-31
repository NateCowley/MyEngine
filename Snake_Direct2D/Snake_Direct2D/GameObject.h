#pragma once

class GameObject
{
public:
	double xPos, yPos;
	double width, height;

	GameObject();
	GameObject(int x, int y);
	GameObject(int x, int y, int w, int h);
	~GameObject();

	virtual void tick(long deltaTime);
};