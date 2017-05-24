#pragma once

#include "Factory.h"

class CTestFactory: public IFactory {
	// Inherited via CFactory
	virtual CScene* GetMainScene(CResourceManager& manager) override;
	virtual CGameController* GetMainController() override;
	virtual IGameResources* GetResources() override;
	virtual CScene* GetScene(int id, CResourceManager& manager) override;
	virtual CGameController* GetController(int id) override;
};