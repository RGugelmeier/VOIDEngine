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
}

//This function gets called the first frame of the scene.
bool GameScene::OnCreate()
{
	Debug::Info("Game Scene started.", "GameScene.cpp", __LINE__);

	sceneInstance = SceneGraph::GetInstance();

	sceneInstance->AddGameObject(new GameObject(vec3(0.0f, 3.0f, 20.0f)), false, "Player");

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(sceneInstance->GetGameObject("Player")->position);
	CoreEngine::GetInstance()->GetCamera()->AddLight(new LightSource(vec3(0.0f, 0.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f, 0.5f));

	CollisionHandler::GetInstance()->OnCreate(100.0f);

	Model* diceModel = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(diceModel);
	Model* tetrahedronModel = new Model("Resources/Models/Tetrahedron.obj", "Resources/Materials/Tetrahedron.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(tetrahedronModel);
	Model* appleModel = new Model("Resources/Models/Apple.obj", "Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(appleModel);

	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(-2.0f, 0.0f, -2.0f)), true, "Cube");
	sceneInstance->AddGameObject(new GameObject(appleModel, vec3(1.5f, 0.0f, 0.0f)), true, "Apple");
	
	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();
	
	sceneInstance->GetGameObject("Cube")->AddComponent<AI>();
	sceneInstance->GetGameObject("Cube")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Cube")->SetRotation(vec3(1.0f, 0.0f, 0.0f));

	sceneInstance->GetGameObject("Cube")->GetComponent<AI>()->KinWander(1.0f, 100.0f);

	//sceneInstance->AddGameObject(new GameObject(tetrahedronModel, vec3(-6.0f, 0.0f, 0.0f)), true, "TetrahedronL");
	//sceneInstance->AddGameObject(new GameObject(tetrahedronModel, vec3(6.0f, 0.0f, 0.0f)), true, "TetrahedronR");
	//
	//sceneInstance->GetGameObject("TetrahedronL")->AddComponent<Physics>();
	//sceneInstance->GetGameObject("TetrahedronR")->AddComponent<Physics>();
	//
	//sceneInstance->GetGameObject("TetrahedronL")->GetComponent<Physics>()->ApplyForce(vec3(0.5f, 0.0f, 0.0f));
	//sceneInstance->GetGameObject("TetrahedronR")->GetComponent<Physics>()->ApplyForce(vec3(-0.5f, 0.0f, 0.0f));

	diceModel = nullptr;
	appleModel = nullptr;

	return true;
}

//This function gets called every tick.
void GameScene::Update(const float deltaTime_)
{
	SceneGraph::GetInstance()->Update(deltaTime_);

	//This is for AI assignment testing
	//Remove these later. Thyis needs to be optimized.
	sceneInstance->GetGameObject("Player")->SetPosition(CoreEngine::GetInstance()->GetCamera()->GetPosition());

	if (testTimer >= 1.0f)
	{
		sceneInstance->GetGameObject("Cube")->GetComponent<AI>()->KinWander(1.0f, 100.0f);
		testTimer = 0.0f;
		cout << "Timer reset" << "\n";
	}
	//sceneInstance->GetGameObject("Cube")->GetComponent<AI>()->Arrive(sceneInstance->GetGameObject("Player")->position, 5.0f, 10.0f, 1.75f, 10.0f, 0.1f);
	//sceneInstance->GetGameObject("Cube")->GetComponent<AI>()->Seek(sceneInstance->GetGameObject("Player")->position, 20.0f);

	

	//This is for physics assignment testing.
	//sceneInstance->GetGameObject("TetrahedronL")->GetComponent<Physics>()->ApplyRotation(sceneInstance->GetGameObject("TetrahedronL")->position, Quaternion(0.1f, 0, 0, 0), vec3(0.0f, 45.0f, 0.0f));
	//sceneInstance->GetGameObject("TetrahedronL")->angle += 0.01f;
	//
	//sceneInstance->GetGameObject("TetrahedronR")->GetComponent<Physics>()->ApplyRotation(sceneInstance->GetGameObject("TetrahedronL")->position, Quaternion(0.1f, 0, 0, 0), vec3(0.0f, 45.0f, 0.0f));
	//sceneInstance->GetGameObject("TetrahedronR")->angle -= 0.01f;
	testTimer += deltaTime_;
}

//This function renders things to the screen.
void GameScene::Render()
{
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}
