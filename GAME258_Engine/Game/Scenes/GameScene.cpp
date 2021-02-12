#include "GameScene.h"

//Set default values.
GameScene::GameScene() : star(nullptr), square(nullptr)
{
	
}
 
//Clean up any used space.
GameScene::~GameScene()
{
	Debug::Info("Game scene ended", "GameScene.cpp", __LINE__);

	delete star;
	star = nullptr;

	delete square;
	square = nullptr;
}

//This function gets called the first frame of the scene.
bool GameScene::OnCreate()
{
	Debug::Info("Game Scene started.", "GameScene.cpp", __LINE__);

	Model* model = new Model();

	Vertex v;
	vector<Vertex> vertexList;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//START STAR VECTOR LIST

	//Triangle one.
	vertexList.reserve(12);
	v.position = vec3(-.75, 0.25f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.0f, 0.25f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.0f, -0.25f, 0.0f);
	vertexList.push_back(v);

	//Triangle two
	v.position = vec3(0.75f, 0.25f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.0f, 0.25f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.0f, -0.25f, 0.0f);
	vertexList.push_back(v);

	//Triangle three
	v.position = vec3(0.0f, 0.75f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(-0.5f, -0.75f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.0f, -0.25f, 0.0f);
	vertexList.push_back(v);

	//Triangle four
	v.position = vec3(0.0f, 0.75f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.5f, -0.75f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.0f, -0.25f, 0.0f);
	vertexList.push_back(v);

	model->AddMesh(new Mesh(vertexList));

	star = new GameObject(model);

	//Clear vertex list to store new shape.
	vertexList.clear();

	//START RECTANGLE VECTOR LIST

	//Triangle one
	vertexList.reserve(6);
	v.position = vec3(0.8, 0.15f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.8f, -0.15f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.95f, -0.15f, 0.0f);
	vertexList.push_back(v);

	//Triangle two
	v.position = vec3(0.8, 0.15f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.95f, 0.15f, 0.0f);
	vertexList.push_back(v);
	v.position = vec3(0.95f, -0.15f, 0.0f);
	vertexList.push_back(v);

	model->AddMesh(new Mesh(vertexList));

	square = new GameObject(model);

	return true;
}

//This function gets called every tick.
void GameScene::Update(const float deltaTime_)
{
}

//This function renders things to the screen.
void GameScene::Render()
{
	star->Render();
}
