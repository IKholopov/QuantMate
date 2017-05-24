#pragma once

#include <Windows.h>

#define OVERLAPPED_WINDOW_CLASSNAME L"OverlappedWClass"
#define OVERLAPPED_WINDOW_TITLE L"OverlappedWindow"
#define OVERLAPPED_MENU_NAME L"OverlappedMenu"

class COverlappedWindow {
public:
	COverlappedWindow();
	virtual ~COverlappedWindow();

	static bool RegisterClass();
	virtual bool Create();
	virtual void Show(int nCmdShow);
	virtual void RunWindow();
protected:
	HWND handle;
	virtual void OnDestroy();
private:
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};