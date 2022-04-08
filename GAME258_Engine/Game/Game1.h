#ifndef GAME1_H
#define GAME1_H

#include "../Engine/Core/CoreEngine.h"
#include "Scenes/StartScene.h"
#include "Scenes/GameScene.h"
#include "Scenes/PhysicsScene.h"
#include "Scenes/DemoScene.h"

//Inherit from GameInterface
class Game1 : public GameInterface
{
public:
	Game1();
	virtual ~Game1();

	bool OnCreate() override;
	void Update(const float deltaTime_) override;
	void Render() override;

	//Getters
	inline const Scene* GetCurrentScene() { return currentScene; }

	//Changes to a new scene.
	inline void SwapScene(CoreEngine::SceneList scene_) { currentSceneName = scene_; }
private:
	CoreEngine::SceneList currentSceneName;
	Scene* currentScene;
	void BuildScene();
};

#endif // !GAME1_H