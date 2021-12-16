#ifndef C_IDLE_H
#define C_IDLE_H

#include "../Condition.h"
class CIdle : public Condition
{
public:
	CIdle(Character* owner_) : Condition(owner_) {}
private:
	bool test() override;
};

#endif // !C_IDLE_H