#ifndef COREENGINE_H
#define COREENGINE_H

#include <memory>
#include "Window.h"
#include "Timer.h"
#include "Debug.h"
#include "GameInterface.h"
#include "Scene.h"

#include "../Rendering/SceneGraph.h"
#include "../Rendering/3D/GameObject.h"

#include "../Graphics/ShaderHandler.h"
#include "../Graphics/TextureHandler.h"
#include "../Graphics/MaterialHandler.h"

#include "../Camera/Camera.h"

#include "../Events/EventListener.h"

#include "../../Physics/Physics.h"

class CoreEngine
{
public:
	//List of scenes.
	enum class SceneList
	{
		START_SCENE,
		GAME_SCENE,
		PHYSICS_SCENE
	};

	//Next 4 lines prevent use of copy or move constructor. Ensures only one engine will exist at any time.
	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	static CoreEngine* GetInstance();

	bool OnCreate(std::string name_, int width_, int height_);
	void Run();
	void Exit();

	//Getters
	inline bool GetIsRunning() const { return isRunning; }
	inline SceneList GetCurrentScene() const { return currentScene; }
	inline float GetScreenWidth() const { return static_cast<float>(window->GetWidth()); }
	inline float GetScreenHeight() const { return static_cast<float>(window->GetHeight()); }
	inline Window* GetWindow() const { return window; }
	inline Camera* GetCamera() const { return camera; }
	inline Timer* GetMainTimer() const { return timer; }
	inline glm::vec2 GetMousePos() {
		//GetMouseState is used here to assign the mouse's current x and y position into the mousePos vec2 values.
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		return glm::ivec2(mousePos.x, mousePos.y);
	}

	//Setters
	void SetGameInterface(GameInterface* gameInterface_);
	void SetCurrentScene(SceneList sceneNum_);
	void SetCamera(Camera* camera_);

	//Mouse event notify functions.
	void NotifyOfMousePressed(ivec2 mouse_, int buttonType_);
	void NotifyOfMouseReleased(ivec2 mouse_, int buttonType_);
	void NotifyOfMouseMove(ivec2 mouse_);
	void NotifyOfMouseScroll(int y_);

	//Keyboard event notify functions.
	void NotifyOfKeyboardPress(SDL_Keycode e_);
	void NotifyOfKeyboardRelease(SDL_Keycode e_);

	//Moves the camera component and object.
	void MoveCamera(vec3 position_);

private:
	CoreEngine();
	~CoreEngine();
	void Update(const float deltaTime_);
	void Render();
	void OnDestroy();

	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>;

	/*MEMBER VARIABLES
	* window = the window that the engine will display.
	* isRunning = a check to see if the engine should be running.
	* timer = the timer that the engine will use to track time and have a consistent speed.
	* fps = the max number of frames per second the engine should run at.
	* gameInterface = the game interface that will be used as a parent class for a game.
	* currentScene = the current scene being used/rendered. See SceneList enum class at the top of this file.
	* mousePos = the mouse's x and y position. Used in the GetMousePos member function.
	*/
	Window* window;
	bool isRunning;

	Timer* timer;
	unsigned int fps;

	GameInterface* gameInterface;

	SceneList currentScene;

	Camera* camera;

	glm::ivec2 mousePos;

	SceneGraph* sceneGraphInstance;
};

#endif // !COREENGINE_H