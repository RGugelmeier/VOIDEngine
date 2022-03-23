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
	Model* planeModel = new Model("Resources/Models/Plane.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(diceModel);
	SceneGraph::GetInstance()->AddModel(planeModel);
	sceneInstance = SceneGraph::GetInstance();

	sceneInstance->AddGameObject(new GameObject(planeModel, vec3(0.0f, 0.0f, 0.0f), false), true, "Cube");
	//sceneInstance->GetGameObject("Cube")->SetScale(vec3(10.0f, 0.5f, 10.0f));
	sceneInstance->GetGameObject("Cube")->SetRotation(vec3(1.0f, 0.0f, 0.0f));

	cout << sceneInstance->GetGameObject("Cube")->GetBoundingBox().minVert.x << " , " << sceneInstance->GetGameObject("Cube")->GetBoundingBox().minVert.y << " , " << sceneInstance->GetGameObject("Cube")->GetBoundingBox().minVert.z << endl;

	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(10.0f, 0.0f, 0.0f), true), true, "Cube2");
	sceneInstance->GetGameObject("Cube2")->SetRotation(vec3(1.0f, 0.0f, 0.0f));

	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(0.0f, 10.0f, 0.0f), false), true, "Player");
	sceneInstance->GetGameObject("Player")->SetRotation(vec3(1.0f, 0.0f, 0.0f));

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(sceneInstance->GetGameObject("Player")->position);
	CoreEngine::GetInstance()->GetCamera()->AddLight(new LightSource(vec3(0.0f, 10.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f, 0.5f));

	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();

	sceneInstance->GetGameObject("Cube")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Cube2")->AddComponent<Physics>();

	testSkybox = new Skybox();
	testSkybox->OnCreate();
	CoreEngine::GetInstance()->GetCamera()->setSkybox(testSkybox);

	//sceneInstance->GetGameObject("Cube")->GetComponent<Physics>()->SetVel(vec3(1.0f, 0.0f, 0.0f));

	diceModel = nullptr;

	return true;
}

//This function gets called every tick.
void GameScene::Update(const float deltaTime_)

{
	//Update scene objects.
	SceneGraph::GetInstance()->Update(deltaTime_);

	//Apply gravity if the player is not standing on top of something.
	//TODO this can probabvly be moved to the Physics component with a check if the parent object should be affected by gravity that is determined by the object.
	if (sceneInstance->GetGameObject("Player")->GetGroundCheck())
	{
		sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->SetAccel(vec3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->ApplyForce(vec3(0.0f, -9.8f, 0.0f));
	}
	cout << sceneInstance->GetGameObject("Player")->GetGroundCheck() << endl;
	//system("CLS");
	//cout << "Max: " << sceneInstance->GetGameObject("Cube")->GetBoundingBox().maxVert.x << " , " << sceneInstance->GetGameObject("Cube")->GetBoundingBox().maxVert.y << " , " << sceneInstance->GetGameObject("Cube")->GetBoundingBox().maxVert.z << endl;
	//cout << "Min: " << sceneInstance->GetGameObject("Cube")->GetBoundingBox().minVert.x << " , " << sceneInstance->GetGameObject("Cube")->GetBoundingBox().minVert.y << " , " << sceneInstance->GetGameObject("Cube")->GetBoundingBox().minVert.z << endl;
	//cout << sceneInstance->GetGameObject("Player")->position.x << " , " << sceneInstance->GetGameObject("Player")->position.y << " , " << sceneInstance->GetGameObject("Player")->position.z << endl;
	//cout << sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->GetVel().x << " , " << sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->GetVel().y << " , " << sceneInstance->GetGameObject("Player")->GetComponent<Physics>()->GetVel().z << endl;
}

//This function renders things to the screen.
void GameScene::Render()
{
	//Render camera.
	CoreEngine::GetInstance()->GetCamera()->Render();

	//Render scene objects.
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}