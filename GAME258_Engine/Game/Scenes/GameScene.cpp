#include "GameScene.h"

//Set default values.
GameScene::GameScene()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}
 
//Clean up any used space.
GameScene::~GameScene()
{
	Debug::Info("Game scene ended", "GameScene.cpp", __LINE__);
	if (testSkybox)
	{
		delete testSkybox;
	}
}

//This function gets called the first frame of the scene.
bool GameScene::OnCreate()
{
	Debug::Info("Game Scene started.", "GameScene.cpp", __LINE__);

	CollisionHandler::GetInstance()->OnCreate(100.0f);

	Model* diceModel = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(diceModel);
	sceneInstance = SceneGraph::GetInstance();

	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(-2.0f, 0.0f, 0.0f)), true, "Cube");
	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(-1.5f, 0.0f, 0.0f)), true, "Cube2");

	sceneInstance->AddGameObject(new GameObject(vec3(0.0f, 0.0f, 20.0f)), false, "Player");

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(sceneInstance->GetGameObject("Player")->position);
	CoreEngine::GetInstance()->GetCamera()->AddLight(new LightSource(vec3(0.0f, 0.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f, 0.5f));

	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();

	sceneInstance->GetGameObject("Cube")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Cube2")->AddComponent<Physics>();

	sceneInstance->GetGameObject("Cube")->SetRotation(vec3(1.0f, 0.0f, 0.0f));
	sceneInstance->GetGameObject("Cube2")->SetRotation(vec3(1.0f, 0.0f, 0.0f));

	testSkybox = new Skybox();
	testSkybox->OnCreate();
	CoreEngine::GetInstance()->GetCamera()->setSkybox(testSkybox);

	sceneInstance->GetGameObject("Cube")->GetComponent<Physics>()->SetVel(vec3(1.0f, 0.0f, 0.0f));

	diceModel = nullptr;

	return true;
}

//This function gets called every tick.
void GameScene::Update(const float deltaTime_)
{
	//Update scene objects.
	SceneGraph::GetInstance()->Update(deltaTime_);
	//Check for collisions in scene.
	//CollisionHandler::GetInstance()->CheckObjCollisions();
	//if (CollisionDetection::GJKDetection(sceneInstance->GetGameObject("Cube"), sceneInstance->GetGameObject("Cube2")))
	//{
	//	cout << "Collision\n";
	//}
	CollisionDetection::GJKDetection(sceneInstance->GetGameObject("Cube"), sceneInstance->GetGameObject("Cube2"));
}

//This function renders things to the screen.
void GameScene::Render()
{
	//Render camera.
	CoreEngine::GetInstance()->GetCamera()->Render();

	//Render scene objects.
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}