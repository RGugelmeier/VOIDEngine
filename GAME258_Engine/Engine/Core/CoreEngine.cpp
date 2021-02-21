#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

//Set window, isRunning, and gameInterface to null as default to make sure that when they are created, there is no junk data. Set fps to 60 and current scene to the start scene as default.
CoreEngine::CoreEngine() : window(nullptr), isRunning(false), fps(60), gameInterface(nullptr), camera(nullptr), timer(nullptr), currentScene(SceneList::START_SCENE)
{
	timer = new Timer();
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

//Called when game starts. Initializes a debug file, a window, and starts the timer and game.
bool CoreEngine::OnCreate(std::string name_, int width_, int height_)
{
	Debug::OnCreate();
	window = new Window();
	if (!window->OnCreate(name_, width_, height_))
	{
		Debug::FatalError("Window failed to initialize.", "CoreEngine.cpp", __LINE__);
		OnDestroy();
		return isRunning = false;
	}

	//Get the shader instance singleton, and create the program with name, and file paths for the vertex and fragment shaders.
	ShaderHandler::GetInstance()->CreateProgram("colourShader", "Engine/Shaders/ColourVertexShader.glsl", "Engine/Shaders/ColourFragmentShader.glsl");

	if (gameInterface)
	{
		if (!gameInterface->OnCreate())
		{
			Debug::FatalError("Game failed to initialize.", "CoreEngine.cpp", __LINE__);
			OnDestroy();
			return isRunning = false;
		}
	}

	Debug::Info("Everything worked", "CoreEngine.cpp", __LINE__);
	timer->Start();
	return isRunning = true;
}

/*Main loop. While game is running, update game and timer, then render screen. Delay the engine by the timer's sleep time to ensure consistent engine speed.
  Whenever the game stops running, call OnDestroy(); */
void CoreEngine::Run()
{
	while (isRunning)
	{
		timer->UpdateFrameTicks();
		Update(timer->GetDeltaTime());
		Render();
		SDL_Delay(timer->GetSleepTime(fps));
	}
	OnDestroy();
}

void CoreEngine::Exit()
{
	isRunning = false;
}

void CoreEngine::SetGameInterface(GameInterface* gameInterface_)
{
	gameInterface = gameInterface_;
}

void CoreEngine::SetCurrentScene(SceneList sceneName_)
{
	currentScene = sceneName_;
}

void CoreEngine::SetCamera(Camera* camera_)
{
	camera = camera_;
}

void CoreEngine::Update(const float deltaTime_)
{
	if (gameInterface)
	{
		gameInterface->Update(deltaTime_);
		std::cout << deltaTime_ << std::endl;
	}
}

//Clear screen to a set colour, render game.
void CoreEngine::Render()
{
	glClearColor(0.0f, 0.00f, 1.00f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gameInterface)
	{
		gameInterface->Render();
	}
	SDL_GL_SwapWindow(window->GetWindow());
}

//Called when game ends. Cleans up memory.
void CoreEngine::OnDestroy()
{
	ShaderHandler::GetInstance()->OnDestroy();

	delete gameInterface;
	gameInterface = nullptr;

	delete camera;
	camera = nullptr;

	delete timer;
	timer = nullptr;

	delete window;
	window = nullptr;
	
	SDL_Quit();
	exit(0);
}
