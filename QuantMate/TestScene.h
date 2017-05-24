#pragma once

#include "Scene.h"
#include "Board.h"

class CTestScene: public CScene {
public:
	CTestScene(CResourceManager& manager);
	~CTestScene();

	CBoard* GetBoard();
private:
	CBoard* board;
};