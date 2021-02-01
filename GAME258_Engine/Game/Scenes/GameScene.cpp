#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	Debug::Info("Start scene ended", "StartScene.cpp", __LINE__);
}

bool GameScene::OnCreate()
{
	Debug::Info("Game Scene started.", "GameScene.cpp", __LINE__);
	return true;
}

void GameScene::Update(const float deltaTime_)
{
	std::cout << deltaTime_ << std::endl;
}

void GameScene::Render()
{
}
