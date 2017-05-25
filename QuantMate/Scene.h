#pragma once

#include <vector>
#include <unordered_map>

#include "SceneObject.h"

class CScene {
public:
	CScene(int width, int height);
	virtual ~CScene();
	void Render(HDC hdc);
	bool OnTimerTick();
	void AddObject(ISceneObject* object, int zIndex);
	void DeleteObject(ISceneObject* object);
protected:
	struct ZIndexedObject {
		ZIndexedObject();
		ZIndexedObject(ISceneObject* object, int zIndex);
		std::unique_ptr<ISceneObject> Object;
		int ZIndex;
	};

	std::unordered_map<ISceneObject*, ZIndexedObject> objects;
	std::multimap<int, ISceneObject*> objectsOrdered;
	int width;
	int height;
};