#pragma once

#include "Graphics.h"
#include "GameObject.h"
#include "EngineTools.h"

class DrawableObject : public GameObject
{
public:
	bool drawFill = true;
	bool isVisible = true;
	D2D1::ColorF color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	RECT hitbox;
	float strokeWidth = 3.0f;
	bool mouseIsDown = false;

	DrawableObject();
	DrawableObject(int x, int y);
	DrawableObject(int x, int y, int w, int h);
	DrawableObject(int x, int y, int w, int h, float r, float g, float b, float a);
	DrawableObject(int x, int y, int w, int h, D2D1::ColorF color);
	~DrawableObject();

	virtual void tick(long deltaTime);
	virtual void draw(Graphics* g);
};