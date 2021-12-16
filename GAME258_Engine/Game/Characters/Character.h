#ifndef CHARACTER_H
#define CHARACTER_H

#include "../../Engine/Rendering/3D/GameObject.h"

class Character : public GameObject
{
public:
	//using GameObject::GameObject;
	Character(Model* model_, vec3 position_ = vec3());
	~Character();

	virtual void Update(const float deltaTime_) = 0;
private:
};

#endif //! CHARACTER_H