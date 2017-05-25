#include "TestFactory.h"
#include "TestScene.h"
#include "TestController.h"
#include "QuantMateResources.h"

CScene* CTestFactory::GetMainScene(CResourceManager& manager, int width, int height)
{
	return new CTestScene(manager, width, height);
}

CGameController* CTestFactory::GetMainController()
{
	return new CTestController();
}

IGameResources* CTestFactory::GetResources()
{
	return new CQuantMateResources();
}

CScene* CTestFactory::GetScene(int id, CResourceManager& manager, int width, int height)
{
	return nullptr;
}

CGameController* CTestFactory::GetController(int id)
{
	return nullptr;
}
