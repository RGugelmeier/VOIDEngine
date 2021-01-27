#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine() : window(nullptr), isRunning(false)
{
}

CoreEngine::~CoreEngine()
{
}

//Sets the engine instance if there is not already one, then returns the engine instance.
CoreEngine* CoreEngine::GetInstance()
{
	if (engineInstance.get() == nullptr)
	{
		engineInstance.reset(new CoreEngine);
	}
	return engineInstance.get();
}

//Called when game starts. Initializes a window and starts the game.
bool CoreEngine::OnCreate(std::string name_, int width_, int height_)
{
	window = new Window();
	if (!window->OnCreate(name_, width_, height_))
	{
		std::cout << "Window failed to initialize." << std::endl;
		OnDestroy();
		return isRunning = false;
	}
	return isRunning = true;
}

//Main loop. While game is running, update game and render screen. Whenever the game stops running, call OnDestroy();
void CoreEngine::Run()
{
	while (isRunning)
	{
		Update(0.016f);
		Render();
	}
	OnDestroy();
}

void CoreEngine::Update(const float deltaTime_)
{
}

//Clear screen, render game.
void CoreEngine::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Call game render
	SDL_GL_SwapWindow(window->GetWindow());
}

//Called when game ends. Cleans up memory.
void CoreEngine::OnDestroy()
{
	delete window;
	window = nullptr;
	SDL_Quit();
	exit(0);
}
