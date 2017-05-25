#include "Notification.h"

CNotification::CNotification(int width, int height) : width(width), height(height)
{
	active = false;
	font = CreateFont(40, 0, 0, 0, 300, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial");
}

CNotification::~CNotification()
{
	DeleteObject(font);
}

void CNotification::Enable(CChessFigure::FigureColor color)
{
	this->color = color;
	active = true;
}

bool CNotification::IsExit(Coordinates position)
{
	if( position.X >= exitRect.left && position.X < exitRect.right &&
		position.Y >= exitRect.top && position.Y < exitRect.bottom ) {
		return true;
	}
	return false;
}

bool CNotification::IsNewGame(Coordinates position)
{
	if( position.X >= newGameRect.left && position.X < newGameRect.right &&
		position.Y >= newGameRect.top && position.Y < newGameRect.bottom ) {
		return true;
	}
	return false;
}

void CNotification::Render(HDC hdc)
{
	if( !active ) {
		return;
	}
	HDC hdcMemory = CreateCompatibleDC(hdc);
	HBITMAP hBM = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hdcMemory, hBM);
	BLENDFUNCTION BlendFunction = { AC_SRC_OVER, 0, 225, 0 };
	SetDCBrushColor(hdcMemory, MENU_BACKGROUND);
	SetDCPenColor(hdcMemory, MENU_BACKGROUND);
	FillRect(hdcMemory, &primalRect, (HBRUSH)GetStockObject(DC_BRUSH));

	SetBkColor(hdcMemory, MENU_BACKGROUND);
	SetTextColor(hdcMemory, MENU_ITEM_TEXT);
	SelectObject(hdcMemory, font);
	if( color == CChessFigure::WHITE ) {
		DrawText(hdcMemory, WHITE_WIN, wcslen(WHITE_WIN), &wonRect, DT_CENTER);
	}
	else {
		DrawText(hdcMemory, BLACK_WIN, wcslen(BLACK_WIN), &wonRect, DT_CENTER);
	}
	
	DrawText(hdcMemory, NEW_GAME, wcslen(NEW_GAME), &newGameRect, DT_LEFT);
	DrawText(hdcMemory, EXIT, wcslen(EXIT), &exitRect, DT_RIGHT);
	AlphaBlend(hdc, 0, 0,
		width, height,
		hdcMemory, 0, 0, width, height, BlendFunction);
	DeleteObject(hBM);
	DeleteDC(hdcMemory);
}

bool CNotification::OnTimerTick()
{
	return false;
}

void CNotification::FetchResources(CResourceManager & manager)
{
}
