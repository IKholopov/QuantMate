#pragma once

#include <Windows.h>

class IDrawable {
	virtual void Draw(HDC hdc) = 0;
};