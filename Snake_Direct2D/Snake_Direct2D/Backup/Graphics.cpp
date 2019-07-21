#include "Graphics.h"

Graphics* Graphics::instance = 0;

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	sBrush = NULL;
}

Graphics::~Graphics()
{

}

void Graphics::Shutdown()
{
	SAFE_RELEASE(factory);
	SAFE_RELEASE(renderTarget);
	SAFE_RELEASE(sBrush);
}

Graphics* Graphics::getInstance()
{
	if (!instance)
	{
		instance = new Graphics();
	}

	return instance;
}

bool Graphics::Init(HWND hWnd)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	if (res != S_OK)
	{
		return false;
	}

	RECT rect;
	GetClientRect(hWnd, &rect);

	res = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)), &renderTarget);

	if (res != S_OK)
	{
		return false;
	}

	res = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &sBrush);

	if (res != S_OK)
	{
		return false;
	}

	return true;
}

void Graphics::BeginDraw()
{
	renderTarget->BeginDraw();
}

void Graphics::EndDraw()
{
	renderTarget->EndDraw();
}

void Graphics::setStrokeWidth(float strokeWidth)
{
	this->strokeWidth = strokeWidth;
}

float Graphics::getStrokeWidth()
{
	return strokeWidth;
}

void Graphics::ClearScreen(D2D1::ColorF color)
{
	ClearScreen(color.r, color.g, color.b);
}

void Graphics::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::DrawCircle(float x, float y, float radius, D2D1::ColorF color, float strokeWidth)
{
	DrawCircle(x, y, radius, color.r, color.g, color.b, color.a, strokeWidth);
}

void Graphics::DrawCircle(float x, float y, float radius, float r, float g, float b, float a, float strokeWidth)
{
	sBrush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x + radius, y + radius), radius, radius), sBrush, strokeWidth);
}

void Graphics::DrawFillCircle(float x, float y, float radius, D2D1::ColorF color)
{
	DrawFillCircle(x, y, radius, color.r, color.g, color.b, color.a);
}

void Graphics::DrawFillCircle(float x, float y, float radius, float r, float g, float b, float a)
{
	sBrush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x + radius, y + radius), radius, radius), sBrush);
}

void Graphics::DrawRect(float x, float y, float w, float h, D2D1::ColorF color, float strokeWidth)
{
	DrawRect(x, y, w, h, color.r, color.g, color.b, color.a, strokeWidth);
}

void Graphics::DrawRect(float x, float y, float w, float h, float r, float g, float b, float a, float strokeWidth)
{
	sBrush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->DrawRectangle(D2D1::Rect(x, y, x + w, y + h), sBrush, strokeWidth);
}

void Graphics::DrawFillRect(float x, float y, float w, float h, D2D1::ColorF color)
{
	DrawFillRect(x, y, w, h, color.r, color.g, color.b, color.a);
}

void Graphics::DrawFillRect(float x, float y, float w, float h, float r, float g, float b, float a)
{
	sBrush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->FillRectangle(D2D1::Rect(x, y, x + w, y + h), sBrush);
}

void Graphics::DrawBitmap(ID2D1Bitmap* bmp, float x, float y, float w, float h, float opacity)
{
	renderTarget->DrawBitmap(bmp, D2D1::RectF(x, y, x + w, y + h), opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height));
}

void Graphics::CreateBitmapFromFile(ID2D1Bitmap** bmp, LPCWSTR fileName)
{
	HRESULT hr;

	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;
	IWICImagingFactory* wicFactory = NULL;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory, 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_IWICImagingFactory, 
		(LPVOID*)&wicFactory);
	if (hr != S_OK)
	{
		goto cleanup;
	}

	hr = wicFactory->CreateDecoderFromFilename(
		fileName, 
		NULL, 
		GENERIC_READ, 
		WICDecodeMetadataCacheOnLoad, 
		&pDecoder);
	if (hr != S_OK)
	{
		goto cleanup;
	}

	hr = pDecoder->GetFrame(0, &pSource);
	if (hr != S_OK)
	{
		goto cleanup;
	}

	hr = wicFactory->CreateFormatConverter(&pConverter);
	if (hr != S_OK)
	{
		goto cleanup;
	}

	hr = pConverter->Initialize(
		pSource, 
		GUID_WICPixelFormat32bppPBGRA, 
		WICBitmapDitherTypeNone, 
		NULL, 
		0.0f, 
		WICBitmapPaletteTypeMedianCut);
	if (hr != S_OK)
	{
		goto cleanup;
	}

	hr = renderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, bmp);
	if (hr != S_OK)
	{
		goto cleanup;
	}

	cleanup:

	if (pDecoder)
	{
		pDecoder->Release();
	}

	if (pSource)
	{
		pSource->Release();
	}

	if (pConverter)
	{
		pConverter->Release();
	}
}

void Graphics::CreateBitmapFromResource(ID2D1Bitmap** bmp, const char* lpType, int lpName)
{
	IWICStream* pIWICStream = NULL;
	IWICBitmapDecoder* pIDecoder = NULL;
	IWICBitmapFrameDecode* pIDecoderFrame = NULL;
	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;
	IWICImagingFactory* wicFactory = NULL;
	IWICFormatConverter* pConverter = NULL;

	imageResHandle = FindResourceEx(
		NULL, 
		(LPCSTR)lpType, 
		MAKEINTRESOURCE(lpName), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
	HRESULT hr = (imageResHandle ? S_OK : E_FAIL);

	if (SUCCEEDED(hr))
	{
		imageResDataHandle = LoadResource(NULL, imageResHandle);
		hr = (imageResDataHandle ? S_OK : E_FAIL);
	}

	if (SUCCEEDED(hr))
	{
		pImageFile = LockResource(imageResDataHandle);
		hr = (pImageFile ? S_OK : E_FAIL);
	}

	if (SUCCEEDED(hr))
	{
		imageFileSize = SizeofResource(NULL, imageResHandle);
		hr = (imageFileSize ? S_OK : E_FAIL);
	}

	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&wicFactory);
	}
	
	if (SUCCEEDED(hr))
	{
		hr = wicFactory->CreateStream(&pIWICStream);
	}

	if (SUCCEEDED(hr))
	{
		hr = pIWICStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile), 
			imageFileSize);
	}

	if (SUCCEEDED(hr))
	{
		hr = wicFactory->CreateDecoderFromStream(
			pIWICStream, 
			NULL, 
			WICDecodeMetadataCacheOnLoad, 
			&pIDecoder);
	}

	if (SUCCEEDED(hr))
	{
		hr = pIDecoder->GetFrame(0, &pIDecoderFrame);
	}

	if (SUCCEEDED(hr))
	{
		hr = wicFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pIDecoderFrame, 
			GUID_WICPixelFormat32bppPBGRA, 
			WICBitmapDitherTypeNone, 
			NULL, 
			0.0f, 
			WICBitmapPaletteTypeMedianCut);
	}

	if (SUCCEEDED(hr))
	{
		hr = renderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, bmp);
	}

	SAFE_RELEASE(pIWICStream);
	SAFE_RELEASE(pIDecoder);
	SAFE_RELEASE(pIDecoderFrame);
	SAFE_RELEASE(wicFactory);
	SAFE_RELEASE(pConverter);
}

ID2D1HwndRenderTarget* Graphics::GetRenderTarget()
{
	return renderTarget;
}