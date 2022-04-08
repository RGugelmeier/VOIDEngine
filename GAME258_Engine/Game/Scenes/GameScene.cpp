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
	Model* wallModel = new Model("Resources/Models/InteriorHouseWall.obj", "Resources/Materials/InteriorHouseWall.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(wallModel);
	Model* tableModel = new Model("Resources/Models/table.obj", "Resources/Materials/table.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(tableModel);
	Model* chairModel = new Model("Resources/Models/Chair.obj", "Resources/Materials/Chair.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(chairModel);

	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(0.0f, 20.0f, 0.0f), false), true, "Player");
	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(0.0f, 0.0f, 0.0f), false), true, "floor");
	sceneInstance->GetGameObject("floor")->SetScale(vec3(10.0f, 0.3f, 10.0f));
	sceneInstance->GetGameObject("floor")->AddComponent<Physics>();
	sceneInstance->AddGameObject(new GameObject(wallModel, vec3(-10.0f, 0.0f, 0.0f), false), false, "wall1");
	//sceneInstance->GetGameObject("wall1")->AddComponent<Physics>();
	sceneInstance->GetGameObject("wall1")->SetScale(vec3(3.0f, 1.5f, 1.0f));
	sceneInstance->GetGameObject("wall1")->SetRotation(vec3(0.0f, 1.0f, 0.0f));
	sceneInstance->GetGameObject("wall1")->SetAngle(radians(90.0f));
	sceneInstance->AddGameObject(new GameObject(wallModel, vec3(10.0f, 0.0f, 0.0f), false), false, "wall2");
	sceneInstance->GetGameObject("wall2")->SetScale(vec3(3.0f, 1.5f, 1.0f));
	//sceneInstance->GetGameObject("wall2")->AddComponent<Physics>();
	sceneInstance->GetGameObject("wall2")->SetRotation(vec3(0.0f, 1.0f, 0.0f));
	sceneInstance->GetGameObject("wall2")->SetAngle(radians(90.0f));
	sceneInstance->AddGameObject(new GameObject(wallModel, vec3(0.0f, 0.0f, -10.0f), false), true, "wall3");
	sceneInstance->GetGameObject("wall3")->SetScale(vec3(3.0f, 1.5f, 1.0f));
	sceneInstance->GetGameObject("wall3")->AddComponent<Physics>();
	sceneInstance->AddGameObject(new GameObject(wallModel, vec3(0.0f, 0.0f, 10.0f), false), true, "wall4");
	sceneInstance->GetGameObject("wall4")->SetScale(vec3(3.0f, 1.5f, 1.0f));
	sceneInstance->GetGameObject("wall4")->AddComponent<Physics>();
	sceneInstance->AddGameObject(new GameObject(tableModel, vec3(3.0f, 1.0f, 0.0f), false), true, "table");
	sceneInstance->GetGameObject("table")->AddComponent<Physics>();
	sceneInstance->AddGameObject(new GameObject(chairModel, vec3(3.0f, 1.0f, 1.0f), false), true, "chair");
	sceneInstance->GetGameObject("chair")->AddComponent<Physics>();

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(vec3(sceneInstance->GetGameObject("Player")->position.x, sceneInstance->GetGameObject("Player")->position.y + 5.0f, sceneInstance->GetGameObject("Player")->position.z));

	CoreEngine::GetInstance()->GetCamera()->AddLight(new DirectionalLight(vec3(-0.2f, -1.0f, -0.3f), normalize(vec3(253.0f, 94.0f, 83.0f)), 0.25f, 0.4f, 0.5f));
	CoreEngine::GetInstance()->GetCamera()->AddLight(new PointLight(vec3(-5.5f, 0.0f, -8.5f), 1.0f, 0.14f, 0.07f, normalize(vec3(233.0f, 150.0f, 122.0f)), 0.05f, 0.0f, 1.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLight(new PointLight(vec3(5.0f, 1.0f, 0.0f), 1.0f, 0.14f, 0.07f, normalize(vec3(233.0f, 150.0f, 122.0f)), 0.05f, 0.0f, 1.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLight(new PointLight(vec3(-5.5f, 1.0f, 8.5f), 1.0f, 0.14f, 0.07f, normalize(vec3(233.0f, 150.0f, 122.0f)), 0.05f, 0.0f, 1.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLight(new PointLight(vec3(-30.5f, 1.0f, 0.0f), 1.0f, 0.14f, 0.07f, normalize(vec3(233.0f, 150.0f, 122.0f)), 0.05f, 0.0f, 1.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLight(new SpotLight(vec3(0.0f, 3.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), cos(radians(30.0f)), cos(radians(39.0f)), 1.0f, 0.09f, 0.032f, vec3(1.0f, 0.9f, 0.7f), 0.0f, 1.0f, 0.0f));

	Skybox* citySkybox = new Skybox();
	citySkybox->OnCreate();
	CoreEngine::GetInstance()->GetCamera()->setSkybox(citySkybox);

	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();

	return true;
}

//This function gets called every tick.
void GameScene::Update(const float deltaTime_)
{
	//Update scene objects.
	SceneGraph::GetInstance()->Update(deltaTime_);

	if (sceneInstance->GetGameObject("Player")->GetGroundCheck())
	{
		sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->SetAccel(vec3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->ApplyForce(vec3(0.0f, -9.8f, 0.0f));
	}

	cout << sceneInstance->GetGameObject("Player")->GetGroundCheck();
}

//This function renders things to the screen.
void GameScene::Render()
{
	//Render camera.
	CoreEngine::GetInstance()->GetCamera()->Render();

	//Render scene objects.
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}