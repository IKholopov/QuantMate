#include "ResourceManager.h"

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	for( auto res = resources.begin(); res != resources.end(); ++res ) {
		res->second->FreeResource();
	}
}

void CResourceManager::RegisterResource(const long resourceId, std::unique_ptr<IResource> resource)
{
	resources.insert(std::pair<const long, std::unique_ptr<IResource>>(resourceId, std::move(resource)));
}

IResource* CResourceManager::GetResource(long resourceId)
{
	return resources.at(resourceId).get();
}
