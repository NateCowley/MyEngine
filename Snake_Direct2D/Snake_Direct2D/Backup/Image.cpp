#include "Image.h"

Image::Image(LPCWSTR filename, Graphics* graphics)
{
	bmp = NULL;

	graphics->CreateBitmapFromFile(&bmp, filename);

	if (bmp)
	{
		width = bmp->GetSize().width;
		height = bmp->GetSize().height;
	}

	xPos = 0;
	yPos = 0;
}

Image::Image(const char* lpName, int lpType, Graphics* graphics)
{
	bmp = NULL;

	graphics->CreateBitmapFromResource(&bmp, lpName, lpType);

	if (bmp)
	{
		width = bmp->GetSize().width;
		height = bmp->GetSize().height;
	}

	xPos = 0;
	yPos = 0;
}

Image::~Image()
{
	SAFE_RELEASE(bmp);
}

void Image::tick(long deltaTime)
{

}

void Image::draw(Graphics* g)
{
	g->DrawBitmap(bmp, xPos, yPos, width, height);
}