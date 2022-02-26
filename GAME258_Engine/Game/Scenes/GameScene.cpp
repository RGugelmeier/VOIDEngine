#include "GameScene.h"
#include <windows.h>
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

	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(0.0f, 0.0f, 0.0f), false), true, "Cube");
	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(5.0f, 5.0f, 5.0f)), true, "Cube2");
            
	
	Model* HouseModel = new Model("Resources/Models/House.obj", "Resources/Materials/House.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(HouseModel);

	Model* HouseModel2 = new Model("Resources/Models/SecondHouse.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(HouseModel2);
	
	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(0.0f, 0.0f, 20.0f), true), true, "Player");

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(sceneInstance->GetGameObject("Player")->position);
	CoreEngine::GetInstance()->GetCamera()->AddLight(new LightSource(vec3(0.0f, 0.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f, 0.5f));

        sceneInstance->AddGameObject(new GameObject(HouseModel, vec3(0.0f, -3.0f, -30.0f)), true, "House");

	sceneInstance->AddGameObject(new GameObject(HouseModel2, vec3(-25.0f, -3.0f, -30.0f)), true, "SecondHouse");
	
	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();

	sceneInstance->GetGameObject("Cube")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Cube2")->AddComponent<Physics>();
	
		
	sceneInstance->GetGameObject("House")->AddComponent<Physics>();
	sceneInstance->GetGameObject("SecondHouse")->AddComponent<Physics>();


	sceneInstance->GetGameObject("Cube")->SetRotation(vec3(1.0f, 0.0f, 0.0f));
	sceneInstance->GetGameObject("Cube2")->SetRotation(vec3(1.0f, 0.0f, 0.0f));
	sceneInstance->GetGameObject("Player")->SetRotation(vec3(1.0f, 0.0f, 0.0f));
	sceneInstance->GetGameObject("House")->SetRotation(vec3(0.0f, 1.0f, 0.0f));
	sceneInstance->GetGameObject("SecondHouse")->SetRotation(vec3(0.0f, 1.0f, 0.0f));

	sceneInstance->GetGameObject("House")->SetAngle(70);
	sceneInstance->GetGameObject("SecondHouse")->SetAngle(70);
	
	
	testSkybox = new Skybox();
	testSkybox->OnCreate();
	CoreEngine::GetInstance()->GetCamera()->setSkybox(testSkybox);

	//sceneInstance->GetGameObject("Cube")->GetComponent<Physics>()->SetVel(vec3(1.0f, 0.0f, 0.0f));
	
	HouseModel = nullptr;
	HouseModel2 = nullptr;
	diceModel = nullptr;

	return true;
}

//This function gets called every tick.
void GameScene::Update(const float deltaTime_)
{
	//Update scene objects.
	SceneGraph::GetInstance()->Update(deltaTime_);
	//Check for collisions in scene.
	sceneInstance->GetGameObject("Cube2")->GetComponent<Physics>()->SetVel(vec3(-1.0f, -0.8f, 0.0f));
	
	CollisionHandler::GetInstance()->CheckObjCollisions();

	cout << sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->GetVel().x << " , " << sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->GetVel().y << " , " << sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->GetVel().z << endl;
	//cout << sceneInstance->GetGameObject("Player")->forward.x << " , " << sceneInstance->GetGameObject("Player")->forward.y << " , " << sceneInstance->GetGameObject("Player")->forward.z << endl;
}

//This function renders things to the screen.
void GameScene::Render()
{
	//Render camera.
	CoreEngine::GetInstance()->GetCamera()->Render();

	//Render scene objects.
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}
