#include "PhysicsScene.h"

//Set default values.
PhysicsScene::PhysicsScene()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

//Clean up any used space.
PhysicsScene::~PhysicsScene()
{
	Debug::Info("Physics scene ended", "PhysicsScene.cpp", __LINE__);
}

bool PhysicsScene::OnCreate()
{
	Debug::Info("Physics Scene started.", "GameScene.cpp", __LINE__);

	//Get the scene graph
	sceneInstance = SceneGraph::GetInstance();

	//Create a player GameObject
	sceneInstance->AddGameObject(new GameObject(vec3(0.0f, 0.0f, 20.0f)), false, "Player");

	//Create a camera and attach it's position to the player game object.
	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(sceneInstance->GetGameObject("Player")->position);

	//Add components to player.
	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();

	//Create a collision handler and set the world size as the parameter.
	CollisionHandler::GetInstance()->OnCreate(100.0f);

	return true;
}

void PhysicsScene::Update(const float deltaTime_)
{
	SceneGraph::GetInstance()->Update(deltaTime_);
	sceneInstance->GetGameObject("Player")->SetPosition(CoreEngine::GetInstance()->GetCamera()->GetPosition());
}

void PhysicsScene::Render()
{
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}