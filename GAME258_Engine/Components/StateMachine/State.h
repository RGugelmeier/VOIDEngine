#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include "Transition.h"

using namespace std;

class Transition;

class State
{
public:
	State(string stateName_);
	~State();
	string GetName() { return stateName; }
	vector<Transition*> GetTransitions() { return transitions; }
	void AddTransition(Transition* transitionToAdd_);
private:
	vector<Transition*> transitions;
	string stateName;
};

#endif // !STATE_H