#pragma once

#include "Scene.h"
#include "GameController.h"
#include "GameResources.h"

class IFactory {
public:
	virtual CScene* GetMainScene(CResourceManager& manager) = 0;
	virtual CGameController* GetMainController() = 0;
	virtual IGameResources* GetResources() = 0;

	virtual CScene* GetScene(int id, CResourceManager& manager) = 0;
	virtual CGameController* GetController(int id) = 0;
};