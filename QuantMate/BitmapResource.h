#pragma once

#include <ObjIdl.h>
#include <gdiplus.h>
#include <string>

#include "Resource.h"

class CBitmapResource: public IResource {
public:
	CBitmapResource(std::wstring resourcePath);
	~CBitmapResource();

	// Inherited via IResource
	void LoadResource() override;
	void FreeResource() override;

	Gdiplus::Bitmap* GetBitmap();

private:
	std::wstring resourcePath;
	Gdiplus::Bitmap* bitmap;
};