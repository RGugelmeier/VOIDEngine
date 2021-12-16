#ifndef STATEMACHINE_H
#define STATEMACHING_H

#include "State.h"
#include "Condition.h"
#include "Transition.h"

#include <vector>
#include <string>

using namespace std;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	string GetCurrentStateName();
	virtual void OnCreate() = 0;
	virtual void Update() = 0;
protected:
	vector<State*> states;
	State *initialState, *currentState;
};

#endif // !STATEMACHINE_H