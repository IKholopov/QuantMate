#pragma once

#include "Scene.h"
#include "Board.h"

class CTestScene: public CScene {
public:
	CTestScene(CResourceManager& manager, int width, int height);
	~CTestScene();

	CBoard* GetBoard();
private:
	CBoard* board;
};