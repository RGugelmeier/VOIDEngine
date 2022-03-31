#include "Game1.h"

//Set currentSceneName to start scene, and current scene to null.
Game1::Game1() : GameInterface(), currentSceneName(CoreEngine::SceneList::START_SCENE), currentScene(nullptr)
{

}

Game1::~Game1()
{
	delete currentScene;
	currentScene = nullptr;
}

//Starts the game by creating the scene. The game must be started on the start scene or else it will stop the program and print a console message.
bool Game1::OnCreate()
{
	if (CoreEngine::GetInstance()->GetCurrentScene() == CoreEngine::SceneList::START_SCENE)
	{
		currentScene = new StartScene();
		currentSceneName = CoreEngine::SceneList::START_SCENE;
		return currentScene->OnCreate();
	}
	Debug::FatalError("Engine's scene is not intialized to start scene. The game must start on the start scene.", "Game1.cpp", __LINE__);
	return false;
}

//Check every update to see if the scene must be changed. If it does, build the new scene.
void Game1::Update(const float deltaTime_)
{
	if (currentSceneName != CoreEngine::GetInstance()->GetCurrentScene())
	{
		BuildScene();
	}
	currentScene->Update(deltaTime_);
}

void Game1::Render()
{
	currentScene->Render();
}

/*Delete the current scene and deallocate the memory.
  Creates a new instance of whatever scene is supposed to be built, changes the game's currentSceneName variable to be correct, and error checks if the scene's OnCreate function ran properly. */
void Game1::BuildScene()
{
	delete currentScene;
	currentScene = nullptr;

	switch (CoreEngine::GetInstance()->GetCurrentScene())
	{
	case CoreEngine::SceneList::PHYSICS_SCENE:
		currentScene = new PhysicsScene();
		break;
	case CoreEngine::SceneList::GAME_SCENE:
		currentScene = new GameScene();
		break;
	case CoreEngine::SceneList::DEMO_SCENE:
		currentScene = new DemoScene();
		break;
	default: //case CoreEngine::SceneList::START_SCENE:
		currentScene = new StartScene();
		break;
	}

	currentSceneName = CoreEngine::GetInstance()->GetCurrentScene();
	if (!currentScene->OnCreate())
	{
		Debug::FatalError("Scene failed to be created.", "Game1.cpp", __LINE__);
		CoreEngine::GetInstance()->Exit();
	}
}
