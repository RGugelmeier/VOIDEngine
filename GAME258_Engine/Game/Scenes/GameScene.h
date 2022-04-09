#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../Engine/Patterns/Container.h"
#include "../../Engine/Core/CoreEngine.h"
#include "../../Physics/Physics.h"
#include "../../Engine/Rendering/Skybox.h"
#include "../../Components/AI.h"

//Inherit from Scene
class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	bool OnCreate() override;
	void Update(const float deltaTime_) override;
	void Render() override;

private:
	SceneGraph* sceneInstance;
	float testTimer = 0.0f;

	Window* window;
	Skybox* testSkybox;
};

#endif // !GAMESCENE_H