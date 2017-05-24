#include "Sprite.h"

CSprite::CSprite(Gdiplus::Bitmap * bitmap)
{
	scale = 1.0f;
	this->spriteBitmap = bitmap;
}

CSprite::CSprite(CSprite& sprite)
{
	this->spriteBitmap = sprite.spriteBitmap;
	this->x = sprite.x;
	this->y = sprite.y;
	this->scale = sprite.scale;
}

CSprite::~CSprite()
{
}

void CSprite::Draw(HDC hdc)
{
	HDC hdcmemory = CreateCompatibleDC(hdc);
	HBITMAP hBM = 0;
	BITMAP bmp;
	spriteBitmap->GetHBITMAP(Gdiplus::Color::Color(0, 0, 0, 0), &hBM);
	SelectObject(hdcmemory, hBM);
	GetObject(hBM, sizeof(bmp), &bmp);
	BLENDFUNCTION BlendFunction;
	BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	BlendFunction.BlendFlags = 0;
	BlendFunction.BlendOp = AC_SRC_OVER;
	BlendFunction.SourceConstantAlpha = 255;
	Gdiplus::RectF rect;
	Gdiplus::Unit unit;
	spriteBitmap->GetBounds(&rect, &unit);
	AlphaBlend(hdc, x - rect.Width*scale/2, y - rect.Height*scale / 2,
			   static_cast<int>(scale * bmp.bmWidth), static_cast<int>(scale * bmp.bmHeight),
			   hdcmemory, 0, 0, bmp.bmWidth, bmp.bmHeight, BlendFunction);
	DeleteObject(hBM);
	DeleteDC(hdcmemory);
}

void CSprite::Draw(HDC hdc, long x, long y)
{
	SetX(x);
	SetY(y);
	Draw(hdc);
}

long CSprite::GetX()
{
	return x;
}

long CSprite::GetY()
{
	return y;
}

void CSprite::SetX(long x)
{
	this->x = x;
}

void CSprite::SetY(long y)
{
	this->y = y;
}

float CSprite::GetScale()
{
	return scale;
}

void CSprite::SetScale(float scale)
{
	this->scale = scale;
}

int CSprite::GetSpriteWidth()
{
	Gdiplus::RectF rect;
	Gdiplus::Unit units;
	spriteBitmap->GetBounds(&rect, &units);
	return rect.Width;
}

int CSprite::GetSpriteHeight()
{
	Gdiplus::RectF rect;
	Gdiplus::Unit units;
	spriteBitmap->GetBounds(&rect, &units);
	return rect.Height;
}

void CSprite::GetBounds(Gdiplus::RectF& rect)
{
	Gdiplus::Unit units;
	spriteBitmap->GetBounds(&rect, &units);
	rect.X += x - rect.Width / 2 - 1;
	rect.Y += y - rect.Height / 2;
}
