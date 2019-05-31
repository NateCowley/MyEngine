#include "GameObject.h"

GameObject::GameObject() : GameObject::GameObject(0, 0, 0, 0)
{

}

GameObject::GameObject(int x, int y) : GameObject::GameObject(x, y, 0, 0)
{

}

GameObject::GameObject(int x, int y, int w, int h)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
}

GameObject::~GameObject()
{

}

void GameObject::tick(long deltaTime)
{

}