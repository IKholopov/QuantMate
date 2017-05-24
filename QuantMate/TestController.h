#pragma once

#include "GameController.h"
#include "TestScene.h"
#include "Board.h"

class CTestController: public CGameController {
	enum GameState {
		ChooseFigure, FigureChosen, ContinuingMove
	};
	
public:
	// Inherited via GameController
	ControllerCommand OnMouseClick(Coordinates coordinates) override;

	void SetScene(CScene* scene) override;

	ControllerCommand Start();
	ControllerCommand Update();
private:
	bool boxSelected;
	CChessFigure::FigureColor color;
	GameState state;
	CBoard* board;
	CChessFigure* pawn;
	std::vector<BoardCell> boundingRects;
	Coordinates selectedPosition;

	void GetBoardRects();
};