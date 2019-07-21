#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <wincodec.h>
#include <d3d11.h>

#include "resource.h"
#include "EngineTools.h"

class Engine;

class Graphics
{
public:
	static Graphics* getInstance();
	~Graphics();

	/// <summary>
	/// Shutdown the Graphics object. This method is used to clean up the Graphics object.
	/// Must be called before destroying the Graphics object.
	/// </summary>
	void Shutdown();

	void setStrokeWidth(float strokeWidth);
	float getStrokeWidth();

	void ClearScreen(D2D1::ColorF color);
	void ClearScreen(float r, float g, float b);

	void DrawCircle(float x, float y, float radius, D2D1::ColorF color, float strokeWidth = 1.0f);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a, float strokeWidth = 1.0f);

	void DrawFillCircle(float x, float y, float radius, D2D1::ColorF color);
	void DrawFillCircle(float x, float y, float radius, float r, float g, float b, float a);

	void DrawRect(float x, float y, float w, float h, D2D1::ColorF color, float strokeWidth = 1.0f);
	void DrawRect(float x, float y, float w, float h, float r, float g, float b, float a, float strokeWidth = 1.0f);

	void DrawFillRect(float x, float y, float w, float h, D2D1::ColorF color);
	void DrawFillRect(float x, float y, float w, float h, float r, float g, float b, float a);

	void DrawBitmap(ID2D1Bitmap* bmp, float x, float y, float w, float h, float opacity = 1.0f);

	void CreateBitmapFromFile(ID2D1Bitmap** bmp, LPCWSTR fileName);
	void CreateBitmapFromResource(ID2D1Bitmap** bmp, const char* lpType, int lpName);

private:
	static Graphics* instance;

	ID2D1Factory* factory; // used to create graphics resources
	ID2D1HwndRenderTarget* renderTarget; // the place in memory where graphics are rendered to
	ID2D1SolidColorBrush* sBrush;

	float strokeWidth = 1.0f;

	friend class Engine;

	Graphics();

	bool Init(HWND hWnd);
	void BeginDraw();
	void EndDraw();
	ID2D1HwndRenderTarget* GetRenderTarget();
};

namespace Color
{
	// PRIMARY COLORS
	const D2D1::ColorF RED(255, 0, 0);
	const D2D1::ColorF BLUE(0, 0, 255);
	const D2D1::ColorF YELLOW(255, 255, 0);
	const D2D1::ColorF BLACK(0, 0, 0);
	const D2D1::ColorF WHITE(255, 255, 255);

	// NON-PRIMARY COLORS
	const D2D1::ColorF GREEN(0, 255, 0);
	const D2D1::ColorF PINK(255, 105, 180);
	const D2D1::ColorF PURPLE(148, 0, 211);
}