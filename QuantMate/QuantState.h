#pragma once

class IQuantState {
public:
	virtual float GetProbability() = 0;
	virtual IQuantState* Split() = 0;
};