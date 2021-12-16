#include "StateMachine.h"

StateMachine::StateMachine()
{
	initialState = nullptr;
	currentState = nullptr;
}

//Delete all states. They will handle cleaning up their own transitions which will handle cleaning up their own conditions.
StateMachine::~StateMachine()
{
	for (State* state : states)
	{
		state = nullptr;
		delete state;
	}
}

string StateMachine::GetCurrentStateName()
{
	return currentState->GetName();
}

void StateMachine::Update()
{
	Transition* triggered = nullptr;

	for (Transition* transition : currentState->GetTransitions())
	{
		if (transition->isTriggered())
		{
			triggered = transition;
			break;
		}
	}
	if (triggered)
	{
		State* targetState = triggered->GetTargetState();

		currentState = targetState;
	}
	else
	{
		//process actions
	}
}