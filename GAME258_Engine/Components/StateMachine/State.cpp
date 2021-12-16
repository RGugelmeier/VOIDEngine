#include "State.h"

State::State(string stateName_)
{
	stateName = stateName_;
}

State::~State()
{
}

void State::AddTransition(Transition* transitionToAdd_)
{
	transitions.push_back(transitionToAdd_);
}
