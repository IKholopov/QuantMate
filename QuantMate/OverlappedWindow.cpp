#include "OverlappedWindow.h"

#include <stdexcept>

COverlappedWindow::COverlappedWindow()
{
	WNDCLASSEX wcx;
	if( !GetClassInfoEx(GetModuleHandle(NULL), OVERLAPPED_WINDOW_CLASSNAME, &wcx) ) {
		throw new std::logic_error("OverlappedWindow class is not registered!\n");
	}
	this->handle = NULL;
}

COverlappedWindow::~COverlappedWindow()
{
}

bool COverlappedWindow::RegisterClass()
{
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = windowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(void*);
	wcx.hInstance = GetModuleHandle(NULL);
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcx.lpszMenuName = OVERLAPPED_MENU_NAME;
	wcx.lpszClassName = OVERLAPPED_WINDOW_CLASSNAME;
	wcx.hIconSm = (HICON)LoadImage(wcx.hInstance, MAKEINTRESOURCE(5), IMAGE_ICON,
		GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CXICON), LR_DEFAULTCOLOR);
	return RegisterClassEx(&wcx);
}

bool COverlappedWindow::Create()
{
	this->handle = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW, OVERLAPPED_WINDOW_CLASSNAME, OVERLAPPED_WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		(HWND)NULL, (HMENU)NULL, GetModuleHandle(NULL), (LPVOID)NULL);
	if( !this->handle ) {
		return false;
	}
	SetWindowLongPtr(this->handle, GWLP_USERDATA, (LONG_PTR)(this));
	return true;
}

void COverlappedWindow::Show(int nCmdShow)
{
	if( !this->handle ) {
		throw new std::logic_error("OverlappedWindow is not created!\n");
	}
	ShowWindow(this->handle, nCmdShow);
	UpdateWindow(this->handle);
}

void COverlappedWindow::RunWindow()
{
	MSG msg;
	int gotMessage;
	while( (gotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && gotMessage != -1 ) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void COverlappedWindow::OnDestroy()
{
	PostQuitMessage(0);
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	COverlappedWindow* window;
	switch( message ) {
	case WM_DESTROY:
		window = (COverlappedWindow*)(GetWindowLongPtr(handle, GWLP_USERDATA));
		window->OnDestroy();
		return 0;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}

