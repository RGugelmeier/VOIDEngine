#include "Transition.h"

Transition::Transition(State* targetState_, Condition* condition_)
{
	targetState = targetState_;
	condition = condition_;
}

Transition::~Transition()
{
}

bool Transition::isTriggered()
{
	return condition->test();
}
