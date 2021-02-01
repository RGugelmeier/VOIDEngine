#include "StartScene.h"

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
	Debug::Info("Start scene ended", "StartScene.cpp", __LINE__);
}

bool StartScene::OnCreate()
{
	Debug::Info("Start Scene started.", "StartScene.cpp", __LINE__);

	CoreEngine::GetInstance()->SetCurrentScene(CoreEngine::SceneList::GAME_SCENE);
	return true;
}

void StartScene::Update(const float deltaTime_)
{
}

void StartScene::Render()
{
}
