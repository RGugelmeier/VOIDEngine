#include "GameScene.h"

//Set default values.
GameScene::GameScene() : cube(nullptr), cubeModel(nullptr)
{
	
}
 
//Clean up any used space.
GameScene::~GameScene()
{
	Debug::Info("Game scene ended", "GameScene.cpp", __LINE__);

	cubeModel = nullptr;

	delete cube;
	cube = nullptr;
}

//This function gets called the first frame of the scene.
bool GameScene::OnCreate()
{
	Debug::Info("Game Scene started.", "GameScene.cpp", __LINE__);

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(vec3(0.0f, 0.0f, 5.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLight(new LightSource(vec3(0.0f, 0.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f, 0.5f));

	cubeModel = new Model("Resources/Models/Apple.obj", "Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));

	//cubeModel->SetScale(vec3(0.5f));
	cube = new GameObject(cubeModel);

	return true;
}

//This function gets called every tick.
void GameScene::Update(const float deltaTime_)
{
	cube->Update(deltaTime_);
}

//This function renders things to the screen.
void GameScene::Render()
{
	//Render objects with the camera passed in so the camera knows how to view this object.
	cube->Render(CoreEngine::GetInstance()->GetCamera());
}
