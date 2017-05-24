#pragma once

#include <vector>
#include "QuantState.h"

class CQuantObject {
public:
	int Collapse();
	void AddState(IQuantState* state);
	std::vector<IQuantState*> GetStates() const;
private:
	std::vector<IQuantState*> states;
};