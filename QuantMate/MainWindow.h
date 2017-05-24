#pragma once

#include <vector>
#include "OverlappedWindow.h"
#include "BitmapResource.h"
#include "Factory.h"

#define WIDTH_DEFAULT 720
#define HEIGHT_DEFAULT 720
#define MAIN_WINDOW_TITLE L"QuantMate"

#define MAIN_TIMER 2048

class CMainWindow : public COverlappedWindow
{
public:
	CMainWindow(IFactory* factory);
	~CMainWindow();

	bool Create() override;
	bool Create(int width, int height);

protected:
	void Render();
	void OnTimer();
	void OnMouseClick(Coordinates coordinates);
	void OnDestroy() override;
private:
	const long dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	HDC bufferDC;
	HBITMAP bufferBitmap;

	int width;
	int height;

	std::unique_ptr<CScene> scene;
	std::unique_ptr<CGameController> controller;
	std::unique_ptr<IFactory> factory;
	CResourceManager resourceManager;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};