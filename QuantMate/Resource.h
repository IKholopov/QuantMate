#pragma once

class IResource {
public:
	virtual void LoadResource() = 0;
	virtual void FreeResource() = 0;
};