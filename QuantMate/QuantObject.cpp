#include "QuantObject.h"

int CQuantObject::Collapse()
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float sum = 0.0f;
	for( int i = 0; i < states.size(); ++i) {
		sum += states[i]->GetProbability();
		if( sum >= r ) {
			auto state = states[i];
			states.clear();
			states.push_back(state);
			return i;
		}
	}
	throw std::logic_error("Sum is not 1 in collapse");
}

void CQuantObject::AddState(IQuantState* state)
{
	states.push_back(state);
}

std::vector<IQuantState*> CQuantObject::GetStates() const
{
	return states;
}
