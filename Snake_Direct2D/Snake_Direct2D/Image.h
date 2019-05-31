#pragma once

#include <d2d1.h>

#include "Graphics.h"
#include "DrawableObject.h"

class Graphics;

/// <summary>
/// Used to display a static image.
/// </summary>
class Image : public DrawableObject
{
public:

	/// <summary>
	/// Creates a bitmap using the image specified with filename.
	/// It is necessary to put an 'L' before the filename parameter 
	/// when passing it in.
	/// </summary>
	Image(LPCWSTR filename, Graphics* graphics);

	/// <summary>
	/// Creates a bitmap using the specified resource.
	/// <para>Example:</para>
	/// Image("MYBITMAP", MAP_IMAGE, graphics)
	/// <para>Explanation:</para>
	/// The name of the object inside the resource (.rc) view, 
	/// <para>the macro assigned by resource.h, </para>
	/// and the Graphics object.
	/// </summary>
	Image(const char* lpName, int lpType, Graphics* graphics);

	~Image();

	virtual void tick(long deltaTime);
	virtual void draw(Graphics* g);

private:
	ID2D1Bitmap* bmp;
};