#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

//Set window, isRunning, and gameInterface to null as default to make sure that when they are created, there is no junk data. Set fps to 60 and current scene to the start scene as default.
CoreEngine::CoreEngine() : window(nullptr), isRunning(false), fps(60), gameInterface(nullptr), camera(nullptr), timer(nullptr), currentScene(SceneList::START_SCENE), sceneGraphInstance(SceneGraph::GetInstance())
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

	//Set the mouse to the centre of the screen.
	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);

	//Register engine to mouse event listener so it can perform it's functions here.
	MouseEventListener::RegisterEngineObject(this);
	KeyboardEventListener::RegisterEngineObject(this);

	//Get the shader instance singleton, and create the program with name, and file paths for the vertex and fragment shaders.
	//Create colour shader program.
	ShaderHandler::GetInstance()->CreateProgram("colourShader", "Engine/Shaders/ColourVertexShader.glsl", "Engine/Shaders/ColourFragmentShader.glsl");
	//Create basic shader program.
	ShaderHandler::GetInstance()->CreateProgram("basicShader", "Engine/Shaders/VertexShader.glsl", "Engine/Shaders/FragmentShader.glsl");
	//Create the skybox shader program;
	ShaderHandler::GetInstance()->CreateProgram("skyboxShader", "Engine/Shaders/SkyboxVert.glsl", "Engine/Shaders/SkyboxFrag.glsl");

	if (gameInterface)
	{
		if (!gameInterface->OnCreate())
		{
			Debug::FatalError("Game failed to initialize.", "CoreEngine.cpp", __LINE__);
			OnDestroy();
			return isRunning = false;
		}
	}

	player = sceneGraphInstance->GetGameObject("Player");

	Debug::Info("Everything worked", "CoreEngine.cpp", __LINE__);
	timer->Start();
	return isRunning = true;
}

/*Main loop. While game is running, update timer, then check for events and update accordingly, the3n update game,
		then render screen. Delay the engine by the timer's sleep time to ensure consistent engine speed.
  Whenever the game stops running, call OnDestroy(); */
void CoreEngine::Run()
{
	while (isRunning)
	{
		timer->UpdateFrameTicks();
		EventListener::Update();
		CollisionHandler::GetInstance()->CheckObjCollisions();
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

void CoreEngine::NotifyOfMousePressed(ivec2 mouse_, int buttonType_)
{
}

void CoreEngine::NotifyOfMouseReleased(ivec2 mouse_, int buttonType_)
{
	//Check for a ray-OBB collision.
	CollisionHandler::GetInstance()->MouseUpdate(mouse_, buttonType_);
}

//Tell the camera to perform movement.
void CoreEngine::NotifyOfMouseMove(ivec2 mouse_)
{
	movespeed = 5.0f;
	float maxSpeed = 1.0f;
	if (camera)
	{
		camera->ProcessMouseMovement(MouseEventListener::GetMouseOffset());

		if (movingLeft)
		{
			float yVel = player->GetComponent<Physics>()->GetVel().y;

			player->GetComponent<Physics>()->SetVel(static_cast<vec3>(-movespeed * player->right, player->GetComponent<Physics>()->GetVel().y, 0.0f));
			player->GetComponent<Physics>()->SetVel(player->GetComponent<Physics>()->GetVel() - (player->right * movespeed));
			
			player->GetComponent<Physics>()->SetVel(vec3(player->GetComponent<Physics>()->GetVel().x, yVel, player->GetComponent<Physics>()->GetVel().z));
		}
		else if (movingForward)
		{
			float yVel = player->GetComponent<Physics>()->GetVel().y;

			player->GetComponent<Physics>()->SetVel(static_cast<vec3>(0.0f, player->GetComponent<Physics>()->GetVel().y, movespeed * vec3(player->forward.x, 0.0f, player->forward.z)));
			player->GetComponent<Physics>()->SetVel(vec3(player->GetComponent<Physics>()->GetVel().x, yVel, player->GetComponent<Physics>()->GetVel().z));
		}
		else if (movingRight)
		{
			float yVel = player->GetComponent<Physics>()->GetVel().y;

			player->GetComponent<Physics>()->SetVel(static_cast<vec3>(movespeed * player->right, player->GetComponent<Physics>()->GetVel().y , 0.0f));
			player->GetComponent<Physics>()->SetVel(player->GetComponent<Physics>()->GetVel() + (player->right * movespeed));

			player->GetComponent<Physics>()->SetVel(vec3(player->GetComponent<Physics>()->GetVel().x, yVel, player->GetComponent<Physics>()->GetVel().z));
		}
		else if (movingBack)
		{
			float yVel = player->GetComponent<Physics>()->GetVel().y;

			player->GetComponent<Physics>()->SetVel(static_cast<vec3>(0.0f, player->GetComponent<Physics>()->GetVel().y, -movespeed * vec3(player->forward.x, 0.0f, player->forward.z)));

			player->GetComponent<Physics>()->SetVel(vec3(player->GetComponent<Physics>()->GetVel().x, yVel, player->GetComponent<Physics>()->GetVel().z));
		}
	}
}

//Notify of when the mouse wheel has been scrolled.
void CoreEngine::NotifyOfMouseScroll(int y_)
{
	//if (camera)
	//{
	//	camera->ProcessMouseZoom(y_);
	//}
}

void CoreEngine::NotifyOfKeyboardPress(SDL_Keycode e_)
{
	player = sceneGraphInstance->GetGameObject("Player");
	movespeed = 5.0f;

	if (camera)
	{
		if (e_ == SDLK_w)
		{
			player->GetComponent<Physics>()->AddVel(static_cast<vec3>(0.0f, 0.0f, movespeed * vec3(player->forward.x, 0.0f, player->forward.z)));
			movingForward = true;
		}
		else if (e_== SDLK_a)
		{
			player->GetComponent<Physics>()->AddVel(static_cast<vec3>(-movespeed * player->right, 0.0f, 0.0f));
			player->GetComponent<Physics>()->AddVel(static_cast<vec3>(player->GetComponent<Physics>()->GetVel().x - (player->right * movespeed), 0.0f, player->GetComponent<Physics>()->GetVel().z - (player->right * movespeed)));
			movingLeft = true;
		}
		else if (e_ == SDLK_s)
		{
			player->GetComponent<Physics>()->AddVel(static_cast<vec3>(0.0f, 0.0f , -movespeed * vec3(player->forward.x, 0.0f, player->forward.z)));
			movingBack = true;
		}
		else if (e_ == SDLK_d)
		{
			player->GetComponent<Physics>()->AddVel(static_cast<vec3>(movespeed * player->right, 0.0f, 0.0f));
			player->GetComponent<Physics>()->AddVel(static_cast<vec3>(player->GetComponent<Physics>()->GetVel().x + (player->right * movespeed), 0.0f, player->GetComponent<Physics>()->GetVel().z + (player->right * movespeed)));
			movingRight = true;
		}
		else if (e_ == SDLK_SPACE)
		{
			player->GetComponent<Physics>()->SetVel(static_cast<vec3>(0.0f, movespeed * player->up, 0.0f));
		}
	}
}

void CoreEngine::NotifyOfKeyboardRelease(SDL_Keycode e_)
{
	if (camera)
	{
		if (e_ == SDLK_w || e_ == SDLK_a || e_ == SDLK_s || e_ == SDLK_d || e_ == SDLK_SPACE)
		{
			sceneGraphInstance->GetGameObject("Player")->GetComponent<Physics>()->SetVel(vec3(0.0f, player->GetComponent<Physics>()->GetVel().y, 0.0f));
			movingForward = movingRight = movingLeft = movingBack = false;
		}
	}
}

void CoreEngine::MoveCamera(vec3 position_)
{
	camera->SetPosition(position_);

	Debug::FatalError("Camera is not attached to a game object named 'Player'. Make sure to name the game object 'Player' before moving the camera.", "CoreEngine.cpp", __LINE__);
}

void CoreEngine::Update(const float deltaTime_)
{
	if (gameInterface)
	{
		gameInterface->Update(deltaTime_);
	}
}

//Clear screen to a set colour, render game.
void CoreEngine::Render()
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
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
	TextureHandler::GetInstance()->OnDestroy();
	MaterialHandler::GetInstance()->OnDestroy();
	CollisionHandler::GetInstance()->OnDestroy();
	SceneGraph::GetInstance()->OnDestroy();

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
