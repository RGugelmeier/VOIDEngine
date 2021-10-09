#ifndef KEYBOARDEVENTLISTENER_H
#define KEYBOARDEVENTLISTENER_H

#include <SDL.h>
#include <glm/glm.hpp>

using namespace glm;

//Forward declare CoreEngine to avoid circular dependencies.
class CoreEngine;
class KeyboardEventListener
{
public:
	//Purely static class. Cannot be created as an object.
	KeyboardEventListener(const KeyboardEventListener&) = delete;
	KeyboardEventListener(KeyboardEventListener&&) = delete;
	KeyboardEventListener& operator=(const KeyboardEventListener&) = delete;
	KeyboardEventListener& operator=(KeyboardEventListener&) = delete;
	KeyboardEventListener() = delete;

	~KeyboardEventListener();

	static void RegisterEngineObject(CoreEngine* engine_);
	static void Update(SDL_Event e_);

	//Notify keyboard events.
	static void NotifyOfButtonPressed(SDL_Keycode e_);

private:
	static CoreEngine* engineInstance;
	static bool firstUpdate;
};

#endif // !KEYBOARDEVENTLISTENER_H