#include "DemoScene.h"

DemoScene::DemoScene()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

DemoScene::~DemoScene()
{
	Debug::Info("Game scene ended", "GameScene.cpp", __LINE__);
	if (citySkybox)
	{
		delete citySkybox;
	}
}

bool DemoScene::OnCreate()
{
	Debug::Info("Game Scene started.", "GameScene.cpp", __LINE__);

	CollisionHandler::GetInstance()->OnCreate(100.0f);

	//Get the scene graph.
	sceneInstance = SceneGraph::GetInstance();

	//Setup cube model to be used by the player.
	Model* diceModel = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(diceModel);

	//Create and set up player.
	sceneInstance->AddGameObject(new GameObject(diceModel, vec3(0.0f, 15.0f, 0.0f), false), true, "Player");

	//Create and set up camera
	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(vec3(sceneInstance->GetGameObject("Player")->position.x, sceneInstance->GetGameObject("Player")->position.y + 5.0f, sceneInstance->GetGameObject("Player")->position.z));
	//CoreEngine::GetInstance()->GetCamera()->AddLight(new LightSource(vec3(0.0f, 40.0f, 20.0f), vec3(0.99f, 0.37f, 0.33f), 0.1f, 0.5f, 0.5f));
	//Material glass(;
	//MaterialHandler::GetInstance()->AddMaterial()
	CoreEngine::GetInstance()->GetCamera()->AddLight(new DirectionalLight(vec3(-0.2f, -1.0f, -0.3f), normalize(vec3(253.0f, 94.0f, 83.0f)), 0.25f, 0.4f, 0.5f));
	CoreEngine::GetInstance()->GetCamera()->AddLight(new PointLight(vec3(5.0f, 0.0f, 0.0f), 1.0f, 0.14f, 0.07f, normalize(vec3(233.0f, 150.0f, 122.0f)), 0.05f, 0.8f, 1.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLight(new SpotLight(vec3(0.0f, 3.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), cos(radians(30.0f)), cos(radians(39.0f)), 1.0f, 0.09f, 0.032f, vec3(1.0f, 0.9f, 0.7f), 0.0f, 1.0f, 0.0f));
	//Add physics and the camera to the player.
	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();

	//Create street model
	streetModel = new Model("Resources/Models/Street.obj", "Resources/Materials/Street.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(streetModel);
	sceneInstance->AddGameObject(new GameObject(streetModel, vec3(44.0f, 0.0f, 0.0f), false), true, "Street1");
	sceneInstance->AddGameObject(new GameObject(streetModel, vec3(0.0f, 0.0f, 0.0f), false), true, "Street5");
	sceneInstance->AddGameObject(new GameObject(streetModel, vec3(22.0f, 0.0f, 0.0f), false), true, "Street2");
	sceneInstance->AddGameObject(new GameObject(streetModel, vec3(-44.0f, 0.0f, 0.0f), false), true, "Street3");
	sceneInstance->AddGameObject(new GameObject(streetModel, vec3(-22.0f, 0.0f, 0.0f), false), true, "Street4");
	//sceneInstance->GetGameObject("Street")->SetPosition(vec3(30.0f, 0.0f, 0.0f));
	//sceneInstance->GetGameObject("Street")->SetScale(vec3(4.0f, 1.0f, 1.0f));
	//Create house model
	houseModel = new Model("Resources/Models/House.obj", "Resources/Materials/House.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(houseModel);
	sceneInstance->AddGameObject(new GameObject(houseModel, vec3(10.0f, 0.0f, -9.3f), false), true, "House1");
	sceneInstance->AddGameObject(new GameObject(houseModel, vec3(-10.0f, 0.0f, -9.3f), false), true, "House2");
	sceneInstance->AddGameObject(new GameObject(houseModel, vec3(10.0f, 0.0f, 9.3f), false), true, "House3");
	sceneInstance->AddGameObject(new GameObject(houseModel, vec3(-10.0f, 0.0f, 9.3f), false), true, "House4");
	sceneInstance->GetGameObject("House3")->SetRotation(vec3(0.0f, 1.0f, 0.0f));
	sceneInstance->GetGameObject("House4")->SetRotation(vec3(0.0f, 1.0f, 0.0f));
	sceneInstance->GetGameObject("House3")->SetAngle(radians(180.0f));
	sceneInstance->GetGameObject("House4")->SetAngle(radians(180.0f));

	streetLightModel = new Model("Resources/Models/lightpost.obj", "Resources/Materials/lightpost.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(streetLightModel);
	sceneInstance->AddGameObject(new GameObject(streetLightModel, vec3(0.0f, 0.0f, -5.0f), false), false, "light1");
	

	//Setup skybox
	citySkybox = new Skybox();
	citySkybox->OnCreate();
	CoreEngine::GetInstance()->GetCamera()->setSkybox(citySkybox);

	//Add physics to the street for collisions.
	sceneInstance->GetGameObject("Street1")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Street2")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Street3")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Street4")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Street5")->AddComponent<Physics>();
	sceneInstance->GetGameObject("House1")->AddComponent<Physics>();
	sceneInstance->GetGameObject("House2")->AddComponent<Physics>();
	sceneInstance->GetGameObject("House3")->AddComponent<Physics>();
	sceneInstance->GetGameObject("House4")->AddComponent<Physics>();

	return true;
}

void DemoScene::Update(const float deltaTime_)
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
	
	cout << "Player pos: " << sceneInstance->GetGameObject("Player")->GetPosition().x << ", " << sceneInstance->GetGameObject("Player")->GetPosition().z << endl;
	//cout << "Player Max: " << sceneInstance->GetGameObject("Player")->GetBoundingBox().maxVert.x << ", " << sceneInstance->GetGameObject("Player")->GetBoundingBox().maxVert.y << " , " << sceneInstance->GetGameObject("Player")->GetBoundingBox().maxVert.z << endl;
	//cout << "Player Min: " << sceneInstance->GetGameObject("Player")->GetBoundingBox().minVert.x << ", " << sceneInstance->GetGameObject("Player")->GetBoundingBox().minVert.y << " , " << sceneInstance->GetGameObject("Player")->GetBoundingBox().minVert.z << endl << endl;
	
	//cout << "Street Max: " << sceneInstance->GetGameObject("Street1")->GetBoundingBox().maxVert.x << ", " << sceneInstance->GetGameObject("Street1")->GetBoundingBox().maxVert.y << " , " << sceneInstance->GetGameObject("Street1")->GetBoundingBox().maxVert.z << endl;
	//cout << "Street Min: " << sceneInstance->GetGameObject("House1")->GetBoundingBox().minVert.x << ", " << sceneInstance->GetGameObject("House1")->GetBoundingBox().minVert.y << " , " << sceneInstance->GetGameObject("House1")->GetBoundingBox().minVert.z << endl << endl;
}

void DemoScene::Render()
{
	//Render camera.
	CoreEngine::GetInstance()->GetCamera()->Render();

	//Render scene objects.
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}
