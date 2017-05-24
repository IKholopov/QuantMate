#pragma once

#include <Windows.h>
#include "ResourceManager.h"

class ISceneObject {
public:
	virtual ~ISceneObject() {};
	virtual void Render(HDC hdc) = 0;
	virtual bool OnTimerTick() = 0;
	virtual void FetchResources(CResourceManager& manager) = 0;
};