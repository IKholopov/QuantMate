#pragma once

#include "Factory.h"

#define SCENE_MAIN 1
#define CONTROLLER_MAIN 1

class CQuantMateFactory : public IFactory {
	// Inherited via CFactory
	virtual CScene* GetMainScene(CResourceManager& manager, int width, int height) override;
	virtual CGameController* GetMainController() override;
	virtual IGameResources* GetResources() override;
	virtual CScene* GetScene(int id, CResourceManager& manager, int width, int height) override;
	virtual CGameController* GetController(int id) override;
};
