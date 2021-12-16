#include "SMCubePerson.h"

SMCubePerson::SMCubePerson(Character* owner_)
{
	owner = owner_;
	OnCreate();
}

SMCubePerson::~SMCubePerson()
{
}

void SMCubePerson::OnCreate()
{
	State* idle = new State("Idle");
	State* persuePlayer = new State("PersuePlayer");

	initialState = idle;
	currentState = initialState;

	//Create a transition and a condition for the idle state to go to the follow player state.
	CFollowPlayer* followCondition;
	followCondition = new CFollowPlayer(owner);
	Transition* idleToFollow = new Transition(persuePlayer, followCondition);
	idle->AddTransition(idleToFollow);

	//Add a condition from follow player to idle. It takes the same condition as the other way but when we test it we will negate the check.
	CIdle* idleCondition;
	idleCondition = new CIdle(owner);
	Transition* followToIdle = new Transition(idle, idleCondition);
	persuePlayer->AddTransition(followToIdle);
}

void SMCubePerson::Update()
{
	StateMachine::Update();
}
