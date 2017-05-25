#include "MainWindow.h"

#include "Coordinates.h"

CMainWindow::CMainWindow(IFactory* factory)
{
	this->factory = std::move(std::unique_ptr<IFactory>(factory));
}

CMainWindow::~CMainWindow()
{
}

bool CMainWindow::Create()
{
	return this->Create(WIDTH_DEFAULT, HEIGHT_DEFAULT);	
}

bool CMainWindow::Create(int width, int height)
{
	this->width = width;
	this->height = height;
	bool status = COverlappedWindow::Create();
	if( status ) {
		SetWindowLongPtr(this->handle, GWLP_WNDPROC, (LONG_PTR)CMainWindow::windowProc);
		SetWindowLong(this->handle, GWL_STYLE, dwStyle);
		SetWindowPos(this->handle, NULL, 0, 0, this->width, this->height, SWP_NOMOVE);
		SetWindowText(this->handle, MAIN_WINDOW_TITLE);

		std::unique_ptr<IGameResources> resources(factory->GetResources());
		resources->InitializeResources(resourceManager);
		scene = std::move(std::unique_ptr<CScene>(factory->GetMainScene(resourceManager, this->width, this->height)));
		controller = std::move(std::unique_ptr<CGameController>(factory->GetMainController()));
		controller->SetScene(scene.get());
		controller->Start();
		SetTimer(handle, MAIN_TIMER, 5, (TIMERPROC)NULL);
	}
	return status;
}

void CMainWindow::Render()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	if( bufferDC == NULL ) {
		bufferDC = CreateCompatibleDC(hdc);
		bufferBitmap = CreateCompatibleBitmap(hdc, width, height);
	}
	HBRUSH brush = static_cast<HBRUSH>(GetStockObject(DC_BRUSH));
	SelectObject(bufferDC, bufferBitmap);
	SetDCBrushColor(bufferDC, RGB(26, 136, 6));
	RECT clientRect;
	GetClientRect(handle, &clientRect);
	FillRect(bufferDC, &clientRect, brush);
	scene->Render(bufferDC);
	BitBlt(hdc, 0, 0, width, height, bufferDC, 0, 0, SRCCOPY);
	EndPaint(handle, &ps);
}

void CMainWindow::OnTimer()
{
	if( scene->OnTimerTick() ) {
		InvalidateRect(handle, NULL, false);
	}
	auto command = controller->Update();
	ProcessCommand(command);
}

void CMainWindow::OnMouseClick(Coordinates coordinates)
{
	if( controller ) {
		ProcessCommand(controller->OnMouseClick(coordinates));
	}
}

void CMainWindow::OnKey(long key)
{
	if( controller ) {
		ProcessCommand(controller->OnKey(key));
	}
}

void CMainWindow::OnDestroy()
{
	KillTimer(handle, MAIN_TIMER);
	COverlappedWindow::OnDestroy();
}

void CMainWindow::ProcessCommand(ControllerCommand& command)
{
	switch( command.Command ) {
	case COMMAND_REDRAW:
		InvalidateRect(handle, NULL, false);
		break;
	case COMMAND_EXIT:
		SendMessage(handle, WM_DESTROY, NULL, NULL);
		break;
	case COMMAND_REDIRECT:
		controller = std::move(std::unique_ptr<CGameController>(factory->GetController(command.ControllerId)));
		scene = std::move(std::unique_ptr<CScene>(factory->GetScene(command.SceneId, resourceManager, width, height)));
		controller->SetScene(scene.get());
		controller->Start();
		break;
	}
}

LRESULT CMainWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	CMainWindow* window = (CMainWindow*)(GetWindowLongPtr(handle, GWLP_USERDATA));
	switch( message ) {
	case WM_PAINT:
		window->Render();
		return 0;
	case WM_TIMER:
		window->OnTimer();
		return 0;
	case WM_DESTROY:
		window->OnDestroy();
		return 0;
	case WM_LBUTTONDOWN:
		window->OnMouseClick(Coordinates(LOWORD(lParam), HIWORD(lParam)));
		return 0;
	case WM_KEYDOWN:
		window->OnKey(wParam);
		return 0;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}
