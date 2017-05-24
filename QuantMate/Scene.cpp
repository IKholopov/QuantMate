#include "Scene.h"

#include <sstream>

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Render(HDC hdc)
{
	for( auto obj = objectsOrdered.begin(); obj != objectsOrdered.end(); ++obj ) {
		obj->second->Render(hdc);
	}
}

bool CScene::OnTimerTick()
{
	bool value = false;
	for( auto obj = objectsOrdered.begin(); obj != objectsOrdered.end(); ++obj ) {
		value |= obj->second->OnTimerTick();
	}
	return value;
}

void CScene::AddObject(ISceneObject* object, int zIndex)
{
	objects.insert(std::make_pair(object, ZIndexedObject(object, zIndex)));
	objectsOrdered.insert(std::make_pair(zIndex, object));
}

void CScene::DeleteObject(ISceneObject* object)
{
	auto iterators = objectsOrdered.equal_range(objects.at(object).ZIndex);
	for( auto it = iterators.first; it != iterators.second; ++it ) {
		if( it->second == object ) {
			objectsOrdered.erase(it);
			break;
		}
	}
	objects.erase(object);
}

CScene::ZIndexedObject::ZIndexedObject()
{
}

CScene::ZIndexedObject::ZIndexedObject(ISceneObject * object, int zIndex): Object(object), ZIndex(zIndex)
{
}
