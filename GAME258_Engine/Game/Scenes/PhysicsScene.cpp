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

	Model* tetrahedronModel = new Model("Resources/Models/Tetrahedron.obj", "Resources/Materials/Tetrahedron.mtl", ShaderHandler::GetInstance()->GetShader("basicShader"));
	SceneGraph::GetInstance()->AddModel(tetrahedronModel);

	sceneInstance = SceneGraph::GetInstance();

	sceneInstance->AddGameObject(new GameObject(vec3(0.0f, 0.0f, 20.0f)), false, "Player");

	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(sceneInstance->GetGameObject("Player")->position);
	CoreEngine::GetInstance()->GetCamera()->AddLight(new LightSource(vec3(0.0f, 0.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f, 0.5f));

	CollisionHandler::GetInstance()->OnCreate(100.0f);

	sceneInstance->GetGameObject("Player")->AddComponent<Physics>();
	sceneInstance->GetGameObject("Player")->AddComponent<Camera>();

	sceneInstance->AddGameObject(new GameObject(tetrahedronModel, vec3(-6.0f, 0.0f, 0.0f)), true, "TetrahedronL");
	sceneInstance->AddGameObject(new GameObject(tetrahedronModel, vec3(6.0f, 0.0f, 0.0f)), true, "TetrahedronR");

	sceneInstance->GetGameObject("TetrahedronL")->AddComponent<Physics>();
	sceneInstance->GetGameObject("TetrahedronR")->AddComponent<Physics>();

	sceneInstance->GetGameObject("TetrahedronL")->GetComponent<Physics>()->ApplyRotation(Quaternion(0.1f, 0, 0, 0), vec3(1.0f, 1.0f, 1.0f));
	sceneInstance->GetGameObject("TetrahedronR")->GetComponent<Physics>()->ApplyRotation(Quaternion(0.1f, 0, 0, 0), vec3(1.0f, 1.0f, 1.0f));
	tetrahedronModel = nullptr;

	return true;
}

void PhysicsScene::Update(const float deltaTime_)
{
	#pragma region MultiThreadExample
	//Discuss threads working.
	//thread th1(&PhysicsScene::TestThread, 1);
	//thread th2(&PhysicsScene::TestThread, 2);
	//
	//th1.join();
	//th2.join();
	#pragma endregion

	sceneInstance->GetGameObject("Player")->SetPosition(CoreEngine::GetInstance()->GetCamera()->GetPosition());

	sceneInstance->GetGameObject("TetrahedronL")->GetComponent<Physics>()->ApplyForce(vec3(0.5f, 0.0f, 0.0f));

	sceneInstance->GetGameObject("TetrahedronL")->angle += 0.01f;
	sceneInstance->GetGameObject("TetrahedronR")->angle += 0.01f;

	if (CollisionDetection::GJKDetection(sceneInstance->GetGameObject("TetrahedronL"), sceneInstance->GetGameObject("TetrahedronR")))
	{
		cout << "Collision detected!\n";
	}

	SceneGraph::GetInstance()->Update(deltaTime_);
}

void PhysicsScene::Render()
{
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}

void PhysicsScene::TestThread(int threadNum)
{
	cout << "Hello from thread: " << threadNum << "\n";
}
