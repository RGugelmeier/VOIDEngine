#ifndef CONDITION_H
#define CONDITION_H

class Character;

class Condition
{
public:
	Condition(Character* owner_);
	~Condition();

	virtual bool test() = 0;
protected:
	Character* owner;
};

#endif // !CONDITION_H