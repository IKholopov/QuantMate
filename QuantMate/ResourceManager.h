#pragma once

#include <map>
#include <memory>
#include "Resource.h"

class CResourceManager {
public:
	CResourceManager();
	~CResourceManager();

	void RegisterResource(const long resourceId, std::unique_ptr<IResource> resource);
	IResource* GetResource(long resourceId);

private:
	std::map<long, std::unique_ptr<IResource>> resources;
};