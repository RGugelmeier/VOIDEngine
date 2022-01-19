#ifndef CUBE_PERSON_H
#define CUBE_PERSON_H
#include "../../Engine/Rendering/SceneGraph.h"
#include "../../Components/AI.h"
#include "../../Physics/Physics.h"
#include "../../Components/StateMachine/StateMachines/SMCubePerson.h"
#include "Character.h"

class CubePerson : public Character
{
public:
	CubePerson(Model* model_, vec3 position_);
	~CubePerson();
	void OnCreate();
	void Update(const float deltaTime_) override;
private:
	SMCubePerson* stateMachine;
	string currentStateName;
};

#endif // !CUBE_PERSON_H