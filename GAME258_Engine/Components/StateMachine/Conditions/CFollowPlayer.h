#ifndef C_FOLLOW_PLAYER_H
#define C_FOLLOW_PLAYER_H

#include "../Condition.h"

class CFollowPlayer : public Condition
{
public:
	CFollowPlayer(Character* owner_) : Condition(owner_) {}
private:
	bool test() override;
};

#endif // !C_FOLLOW_PLAYER_H