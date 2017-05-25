#pragma once

#include "Coordinates.h"
#include "SceneObject.h"
#include "ChessFigure.h"

#define MENU_BACKGROUND RGB(32, 26, 94)
#define MENU_ITEM RGB(0, 0, 0)
#define MENU_ITEM_TEXT RGB(255, 255, 255)
#define WHITE_WIN L"White won!"
#define BLACK_WIN L"Black won!"
#define NEW_GAME L"New Game"
#define EXIT L"Exit"

class CNotification : public ISceneObject
{
public:
	CNotification(int width, int height);
	~CNotification();
	void Enable(CChessFigure::FigureColor color);

	bool IsNewGame(Coordinates position);
	bool IsExit(Coordinates position);

	// Inherited via ISceneObject
	virtual void Render(HDC hdc) override;
	virtual bool OnTimerTick() override;
	virtual void FetchResources(CResourceManager & manager) override;
private:
	int width;
	int height;
	bool active;
	HFONT font;

	CChessFigure::FigureColor color;
	RECT primalRect = { width / 4, height / 4, 3 * width / 4, 3 * height / 4 };
	RECT wonRect = { width / 4, height / 4 + 20, 3 * width / 4, 3 * height / 4 };
	RECT newGameRect = { width / 4, 3 * height / 4 - 40, width / 4 + 200, 3 * height / 4 };
	RECT exitRect = { 3 * width / 4 - 200, 3 * height / 4 - 40, 3 * width / 4, 3 * height / 4 };
};