#include "Image.h"

Image::Image()
{

}

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

Image::Image(std::string filename, Graphics* graphics) : Image::Image(EngineTools::strToLPCWSTR(filename), graphics)
{
	
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
	g->DrawImage(bmp, xPos, yPos, width, height);
}