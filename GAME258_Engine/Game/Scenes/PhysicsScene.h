#ifndef PHYSICSSCENE_H
#define PHYSICSSCENE_H

#include "../../Engine/Core/CoreEngine.h"
#include "../../Physics/Physics.h"
#include "../../Components/Grid.h"

//Inherit from Scene
class PhysicsScene : public Scene
{
public:
	PhysicsScene();
	virtual ~PhysicsScene();

	bool OnCreate() override;
	void Update(const float deltaTime_) override;
	void Render() override;

private:
	SceneGraph* sceneInstance;

	Window* window;
};

#endif // !GAMESCENE_H