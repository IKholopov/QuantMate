#include "TestFactory.h"
#include "TestScene.h"
#include "TestController.h"
#include "QuantMateResources.h"

CScene* CTestFactory::GetMainScene(CResourceManager& manager)
{
	return new CTestScene(manager);
}

CGameController* CTestFactory::GetMainController()
{
	return new CTestController();
}

IGameResources* CTestFactory::GetResources()
{
	return new CQuantMateResources();
}

CScene* CTestFactory::GetScene(int id, CResourceManager& manager)
{
	return nullptr;
}

CGameController* CTestFactory::GetController(int id)
{
	return nullptr;
}
