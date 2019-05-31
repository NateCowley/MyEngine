#include "DrawableObject.h"

DrawableObject::DrawableObject() : DrawableObject(0, 0, 0, 0, Color::BLACK)
{

}

DrawableObject::DrawableObject(int x, int y) : DrawableObject(x, y, 0, 0, Color::BLACK)
{

}

DrawableObject::DrawableObject(int x, int y, int w, int h) : DrawableObject(x, y, w, h, Color::BLACK)
{

}

DrawableObject::DrawableObject(int x, int y, int w, int h, float r, float g, float b, float a) : DrawableObject(x, y, w, h, D2D1::ColorF(r, g, b, a))
{

}

DrawableObject::DrawableObject(int x, int y, int w, int h, D2D1::ColorF color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f))
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	this->color = color;
	hitbox = { x, y, w, h };
}

DrawableObject::~DrawableObject()
{

}

void DrawableObject::tick(long deltaTime)
{

}

void DrawableObject::draw(Graphics* g)
{

}