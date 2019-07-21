#include "SpriteSheet.h"

SpriteSheet::SpriteSheet()
{

}

SpriteSheet::SpriteSheet(LPCWSTR filename, Graphics* graphics) : Image(filename, graphics)
{
	
}

SpriteSheet::SpriteSheet(LPCWSTR filename, Graphics* graphics, int width, int height) : Image(filename, graphics)
{
	spriteWidth = width;
	spriteHeight = height;

	spritesAcross = this->width / spriteWidth;
}

SpriteSheet::~SpriteSheet()
{

}

void SpriteSheet::tick(long deltaTime)
{
	spriteIndex += 1;

	if (spriteIndex > 5)
	{
		spriteIndex = 0;
	}


}

void SpriteSheet::draw(Graphics* g)
{
	float indexOffsetX = (float)(spriteIndex % spritesAcross);
	float indexOffsetY = (float)(spriteIndex / spritesAcross);

	g->DrawSpriteSheet(bmp, xPos, yPos, xPos + spriteWidth, yPos + spriteHeight, indexOffsetX * spriteWidth, indexOffsetY * spriteHeight, indexOffsetX * spriteWidth + spriteWidth, indexOffsetY * spriteHeight + spriteHeight);
}