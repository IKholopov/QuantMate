#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>

#include "MainWindow.h"
#include "QuantMateFactory.h"

int RunMainWindow(int nCmdShow)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CMainWindow::RegisterClass();
	CMainWindow window(new CQuantMateFactory());
	if( !window.Create(720, 720) ) {
		MessageBox(NULL, L"Failed to create MainWindow", MAIN_WINDOW_TITLE, MB_OK);
		return 1;
	}
	window.Show(nCmdShow);
	window.RunWindow();
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int status = RunMainWindow(nCmdShow);

	return status;
}