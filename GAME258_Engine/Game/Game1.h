#ifndef GAME1_H
#define GAME1_H

#include "../Engine/Core/CoreEngine.h"
#include "Scenes/StartScene.h"
#include "Scenes/GameScene.h"
#include "Scenes/PhysicsScene.h"

//Inherit from GameInterface
class Game1 : public GameInterface
{
public:
	Game1();
	virtual ~Game1();

	bool OnCreate() override;
	void Update(const float deltaTime_) override;
	void Render() override;
private:
	CoreEngine::SceneList currentSceneName;
	Scene* currentScene;
	void BuildScene();
};

#endif // !GAME1_H