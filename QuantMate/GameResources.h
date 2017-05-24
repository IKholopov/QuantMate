#pragma once

#include "ResourceManager.h"

class IGameResources {
public:
	virtual void InitializeResources(CResourceManager& manager) = 0;
};