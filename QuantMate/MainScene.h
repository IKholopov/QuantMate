#pragma once

#include "Scene.h"
#include "Board.h"
#include "PauseMenu.h"
#include "Notification.h"

class CMainScene : public CScene {
public:
	CMainScene(CResourceManager& manager, int width, int height);
	~CMainScene();

	CBoard* GetBoard();
	CPauseMenu* GetPauseMenu();
	CNotification* GetNotification();
private:
	CBoard* board;
	CPauseMenu* menu;
	CNotification* notification;
};