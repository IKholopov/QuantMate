#pragma once

#include "GameController.h"
#include "MainScene.h"
#include "Board.h"

class CQuantMateController : public CGameController {
	enum GameState {
		ChooseFigure, FigureChosen, ContinuingMove
	};

public:
	// Inherited via GameController
	ControllerCommand OnMouseClick(Coordinates coordinates) override;
	ControllerCommand OnKey(long key) override;

	void SetScene(CScene* scene) override;

	ControllerCommand Start();
	ControllerCommand Update();
private:
	bool boxSelected;
	bool paused;
	bool finished;
	CChessFigure::FigureColor color;
	GameState state;
	CBoard* board;
	CPauseMenu* menu;
	CNotification* notification;
	CChessFigure* pawn;
	std::vector<BoardCell> boundingRects;
	Coordinates selectedPosition;
	void SwitchPause();
	void OnWin(CChessFigure::FigureColor color);

	void GetBoardRects();
};