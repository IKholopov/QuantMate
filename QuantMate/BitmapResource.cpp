#include "BitmapResource.h"

CBitmapResource::CBitmapResource(std::wstring resourcePath)
{
	this->resourcePath = resourcePath;
	this->bitmap = NULL;
}

CBitmapResource::~CBitmapResource()
{
	if( bitmap ) {
		FreeResource();
	}
}

void CBitmapResource::LoadResource()
{
	bitmap = Gdiplus::Bitmap::FromFile(resourcePath.c_str());
	if( !bitmap ) {
		throw std::invalid_argument("Failed to load resource");
	}
}

void CBitmapResource::FreeResource()
{
	if( bitmap ) {
		delete bitmap;
		bitmap = NULL;
	}
}

Gdiplus::Bitmap * CBitmapResource::GetBitmap()
{
	if( !bitmap ) {
		LoadResource();
	}
	return bitmap;
}
