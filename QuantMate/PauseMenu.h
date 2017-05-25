#pragma once

#include "Coordinates.h"
#include "SceneObject.h"

#define MENU_BACKGROUND RGB(32, 26, 94)
#define MENU_ITEM RGB(0, 0, 0)
#define MENU_ITEM_TEXT RGB(255, 255, 255)
#define NEW_GAME L"New Game"
#define CONTINUE L"Continue"

class CPauseMenu : public ISceneObject 
{
public:
	CPauseMenu(int width, int height);
	~CPauseMenu();
	void Enable();
	void Disable();

	bool IsContinue(Coordinates position);
	bool IsNewGame(Coordinates position);

	// Inherited via ISceneObject
	virtual void Render(HDC hdc) override;
	virtual bool OnTimerTick() override;
	virtual void FetchResources(CResourceManager & manager) override;
private:
	int width;
	int height;
	bool active;
	HFONT font;
	RECT newGameRect = { width / 2 - 100, height / 2 - 10 + 20, width / 2 + 100, height / 2 + 10 + 20};
	RECT continueRect = { width / 2 - 100, height / 2 - 10 - 20, width / 2 + 100, height / 2 + 10 - 20};
};