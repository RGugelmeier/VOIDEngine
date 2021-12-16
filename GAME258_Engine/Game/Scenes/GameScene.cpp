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

	Model* diceModel = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(diceModel);

	Model* appleModel = new Model("Resources/Models/Apple.obj", "Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(appleModel);

	sceneInstance->AddGameObject(new CubePerson(diceModel, vec3(-2.0f, 0.0f, 0.0f)), true, "Cube");

	sceneInstance = SceneGraph::GetInstance();

	sceneInstance->AddGameObject(new GameObject(vec3(0.0f, 0.0f, 20.0f)), false, "Player");

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(sceneInstance->GetGameObject("Player")->position);
	CoreEngine::GetInstance()->GetCamera()->AddLight(new LightSource(vec3(0.0f, 0.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f, 0.5f));

	CollisionHandler::GetInstance()->OnCreate(100.0f);

	//sceneInstance->AddGameObject(new GameObject(diceModel, vec3(-2.0f, 0.0f, -2.0f)), true, "Cube");
	sceneInstance->AddGameObject(new GameObject(appleModel, vec3(1.5f, 0.0f, 0.0f)), true, "Apple");
	
	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();
	
	sceneInstance->GetGameObject("Cube")->AddComponent<AI>();
	sceneInstance->GetGameObject("Cube")->AddComponent<Physics>();

	sceneInstance->GetGameObject("Cube")->SetRotation(vec3(1.0f, 0.0f, 0.0f));
	sceneInstance->GetGameObject("Apple")->SetRotation(vec3(1.0f, 0.0f, 0.0f));

	Grid levelGrid = Grid(1.0f, vec3(10, 10, 10));

	sceneInstance->AddGameObject(new GameObject(vec3(0.0f, 0.0f, 0.0f)), false, "Graph");
	sceneInstance->GetGameObject("Graph")->AddComponent<Grid>(levelGrid);

	sceneInstance->GetGameObject("Cube")->GetComponent<AI>()->AStar(sceneInstance->GetGameObject("Graph")->GetComponent<Grid>()->getGraph(), 0, 35);

	diceModel = nullptr;
	appleModel = nullptr;

	return true;
}

//This function gets called every tick.
void GameScene::Update(const float deltaTime_)
{
	SceneGraph::GetInstance()->Update(deltaTime_);
	CoreEngine::GetInstance()->GetCamera()->SetPosition(sceneInstance->GetGameObject("Player")->position);
	//cout << "Distance: " << distance(sceneInstance->GetGameObject("Cube")->position, sceneInstance->GetGameObject("Player")->position) << "\n";
	//cout << "Cube pos: " << sceneInstance->GetGameObject("Cube")->position.x << ", " << sceneInstance->GetGameObject("Cube")->position.y << ", " << sceneInstance->GetGameObject("Cube")->position.z << "\n";
	//cout << "Player pos: " << sceneInstance->GetGameObject("Player")->position.x << ", " << sceneInstance->GetGameObject("Player")->position.y << ", " << sceneInstance->GetGameObject("Player")->position.z << "\n";
	//sceneInstance->GetGameObject("Cube")->GetComponent<AI>()->Arrive(sceneInstance->GetGameObject("Player")->position, 5.0f, 10.0f, 5.0f, 10.0f, 0.1f);
	//sceneInstance->GetGameObject("Cube")->GetComponent<AI>()->Seek(sceneInstance->GetGameObject("Player")->position, 20.0f);
}

//This function renders things to the screen.
void GameScene::Render()
{
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}