#ifndef SMCUBEPERSON_H
#define SMCUBEPERSON_H

#include "../StateMachine.h"
#include "../Conditions/CFollowPlayer.h"
#include "../Conditions/CIdle.h"

class SMCubePerson : public StateMachine
{
public:
	SMCubePerson(Character* owner_);
	~SMCubePerson();

	void OnCreate() override;
	void Update() override;
private:
	Character* owner;
};

#endif // !SMCUBEPERSON_H