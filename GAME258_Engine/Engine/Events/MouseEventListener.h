#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include <SDL.h>
#include <glm/glm.hpp>

using namespace glm;

//Forward declare CoreEngine to avoid circular dependencies.
class CoreEngine;
class MouseEventListener {
public:
	//Purely static class. Cannot be created as an object.
	MouseEventListener(const MouseEventListener&) = delete;
	MouseEventListener(MouseEventListener&&) = delete;
	MouseEventListener& operator=(const MouseEventListener&) = delete;
	MouseEventListener& operator=(MouseEventListener&) = delete;
	MouseEventListener() = delete;

	~MouseEventListener();

	static void RegisterEngineObject(CoreEngine* engine_);
	static void Update(SDL_Event e_);

	//Notify mouse events.
	static void NotifyOfMousePressed(int buttonType_);
	static void NotifyOfMouseReleased(int buttonType_);
	static void NotifyOfMouseMove();
	static void NotifyOfMouseScroll(int y_);

	//ivec is an integer vector.
	//Mouse data getters.
	static ivec2 GetPreviousMousePosition();
	static ivec2 GetMousePosition();
	static ivec2 GetMouseOffset();

private:
	static CoreEngine* engineInstance;
	static ivec2 mouse, prevMouse;
	static bool firstUpdate;
	static void UpdateMousePosition();
};

#endif // !MOUSEVENTLISTENER_H