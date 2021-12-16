#include "CubePerson.h"

CubePerson::CubePerson(Model* model_, vec3 position_) : Character(model_, position_)
{
	OnCreate();
}

CubePerson::~CubePerson()
{
}

void CubePerson::OnCreate()
{
	stateMachine = new SMCubePerson(this);
}

void CubePerson::Update(const float deltaTime_)
{
	GameObject::Update(deltaTime_);
	stateMachine->Update();

	currentStateName = stateMachine->GetCurrentStateName();

	if (currentStateName == "Idle")
	{
		GetComponent<Physics>()->SetAccel(vec3(0.0f, 0.0f, 0.0f));
		GetComponent<Physics>()->SetVel(vec3(0.0f, 0.0f, 0.0f));
	}
	else if (currentStateName == "PersuePlayer")
	{
		GetComponent<AI>()->Arrive(SceneGraph::GetInstance()->GetGameObject("Player")->position, 50.0f, 30.0f, 5.0f, 6.0f, 0.1f);
	}
}