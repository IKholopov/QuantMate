#pragma once

#include <objidl.h>
#include <gdiplus.h>

#include "Drawable.h"

class CSprite : public IDrawable {
public:
	CSprite(Gdiplus::Bitmap* bitmap);
	CSprite(CSprite& sprite);
	virtual ~CSprite();
	// Inherited via IDrawable
	void Draw(HDC hdc) override;
	void Draw(HDC hdc, long x, long y);
	long GetX();
	long GetY();
	void SetX(long x);
	void SetY(long y);
	float GetScale();
	void SetScale(float scale);
	int GetSpriteWidth();
	int GetSpriteHeight();
	void GetBounds(Gdiplus::RectF& rect);
private:
	Gdiplus::Bitmap* spriteBitmap;
	long x;
	long y;
	float scale;
};