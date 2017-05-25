#include "PauseMenu.h"

CPauseMenu::CPauseMenu(int width, int height): width(width), height(height)
{
	active = false;
	font = CreateFont(25, 0, 0, 0, 300, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial");
}

CPauseMenu::~CPauseMenu()
{
	DeleteObject(font);
}

void CPauseMenu::Enable()
{
	active = true;
}

void CPauseMenu::Disable()
{
	active = false;
}

bool CPauseMenu::IsContinue(Coordinates position)
{
	if( position.X >= continueRect.left && position.X < continueRect.right &&
		position.Y >= continueRect.top && position.Y < continueRect.bottom ) {
		return true;
	}
	return false;
}

bool CPauseMenu::IsNewGame(Coordinates position)
{
	if( position.X >= newGameRect.left && position.X < newGameRect.right &&
		position.Y >= newGameRect.top && position.Y < newGameRect.bottom ) {
		return true;
	}
	return false;
}

void CPauseMenu::Render(HDC hdc)
{
	if( !active ) {
		return;
	}
	HDC hdcMemory = CreateCompatibleDC(hdc);
	HBITMAP hBM = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hdcMemory, hBM);
	BLENDFUNCTION BlendFunction = { AC_SRC_OVER, 0, 127, 0 };
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	SetDCBrushColor(hdcMemory, MENU_BACKGROUND);
	SetDCPenColor(hdcMemory, MENU_BACKGROUND);
	FillRect(hdcMemory, &rect, (HBRUSH)GetStockObject(DC_BRUSH));
	
	SetBkColor(hdcMemory, MENU_ITEM);
	SetTextColor(hdcMemory, MENU_ITEM_TEXT);
	SelectObject(hdcMemory, font);
	DrawText(hdcMemory, NEW_GAME, wcslen(NEW_GAME), &newGameRect, DT_CENTER);
	DrawText(hdcMemory, CONTINUE, wcslen(CONTINUE), &continueRect, DT_CENTER);
	AlphaBlend(hdc, 0, 0,
		width, height,
		hdcMemory, 0, 0, width, height, BlendFunction);
	DeleteObject(hBM);
	DeleteDC(hdcMemory);
}

bool CPauseMenu::OnTimerTick()
{
	return false;
}

void CPauseMenu::FetchResources(CResourceManager & manager)
{
}
