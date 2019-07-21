#pragma once

#include "Image.h"

class Graphics;

class SpriteSheet : public Image
{
public:
	int spriteWidth, spriteHeight, spritesAcross;
	int spriteX = 0, spriteY = 0;
	int spriteIndex = 0;
	
	SpriteSheet();
	SpriteSheet(LPCWSTR filename, Graphics* graphics);
	SpriteSheet(LPCWSTR filename, Graphics* graphics, int width, int height);
	~SpriteSheet();

	virtual void tick(long deltaTime);
	virtual void draw(Graphics* g);
};