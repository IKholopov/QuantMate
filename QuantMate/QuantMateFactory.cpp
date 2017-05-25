#include "QuantMateFactory.h"

#include "MainScene.h"
#include "QuantMateController.h"

CScene* CQuantMateFactory::GetMainScene(CResourceManager& manager, int width, int height)
{
	return new CMainScene(manager, width, height);
}

CGameController* CQuantMateFactory::GetMainController()
{
	return new CQuantMateController();
}

IGameResources* CQuantMateFactory::GetResources()
{
	return new CQuantMateResources();
}

CScene* CQuantMateFactory::GetScene(int id, CResourceManager& manager, int width, int height)
{
	if( id == SCENE_MAIN ){
		return new CMainScene(manager, width, height);
	} 
	return nullptr;
}

CGameController* CQuantMateFactory::GetController(int id)
{
	if( id == CONTROLLER_MAIN ) {
		return new CQuantMateController();
	}
	return nullptr;
}

