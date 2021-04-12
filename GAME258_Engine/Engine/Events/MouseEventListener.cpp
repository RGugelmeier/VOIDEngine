#include "MouseEventListener.h"
//CoreEngine needs to be included in the cpp to avoid circular dependencies. This goes hand in hand with the forward declaration in MouseEventListener.h.
#include "../Core/CoreEngine.h"

//Redeclare and define static members.
CoreEngine* MouseEventListener::engineInstance = nullptr;
ivec2 MouseEventListener::mouse = ivec2();
ivec2 MouseEventListener::prevMouse = ivec2();
bool MouseEventListener::firstUpdate = true;

//Clean up memory used.
MouseEventListener::~MouseEventListener()
{
	engineInstance = nullptr;
}

//Get a reference to the engine.
void MouseEventListener::RegisterEngineObject(CoreEngine* engine_)
{
	engineInstance = engine_;
}

//Perform an action depending on what mouse button is pressed.
//Update mouse position is called on each action to keep the mouse position accurate each check.
void MouseEventListener::Update(SDL_Event e_)
{
	if (e_.type == SDL_MOUSEBUTTONDOWN)
	{
		UpdateMousePosition();
		NotifyOfMousePressed(e_.button.button);
	}
	else if (e_.type == SDL_MOUSEBUTTONUP)
	{
		UpdateMousePosition();
		NotifyOfMouseReleased(e_.button.button);
	}
	else if (e_.type == SDL_MOUSEMOTION)
	{
		UpdateMousePosition();
		NotifyOfMouseMove();
	}
	else if (e_.type == SDL_MOUSEWHEEL)
	{
		UpdateMousePosition();
		NotifyOfMouseScroll(e_.wheel.y); 
	}
}

//For each notify function, call the engine's notify function. This just passes variables from the mouse and event.
void MouseEventListener::NotifyOfMousePressed(int buttonType_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMousePressed(mouse, buttonType_);
	}
}

void MouseEventListener::NotifyOfMouseReleased(int buttonType_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseReleased(mouse, buttonType_);
	}
}

void MouseEventListener::NotifyOfMouseMove()
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseMove(mouse);
	}
}

void MouseEventListener::NotifyOfMouseScroll(int y_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseScroll(y_);
	}
}

//Getters.
ivec2 MouseEventListener::GetPreviousMousePosition()
{
	return prevMouse;;
}

ivec2 MouseEventListener::GetMousePosition()
{
	return mouse;
}

//the y value is flipper because the bottom of the screen is supposed to be 0 due to how SDL works with y screen coords.
ivec2 MouseEventListener::GetMouseOffset()
{
	return ivec2(mouse.x - prevMouse.x, prevMouse.y - mouse.y);
}
//End getters

//Update the mouse position data.
void MouseEventListener::UpdateMousePosition()
{
	//Create temp x and y variables.
	int tmpX, tmpY;
	//Pass temp variables into sdl function to set the variables to be the current mouse position.
	SDL_GetMouseState(&tmpX, &tmpY);
	//Get the y position of the mouse properly by subtracting the y position variable from the top of the screen. Again this is due to wanting 0 y to be the bottom of the screen.
	//Cast this to an int.
	tmpY = static_cast<int>(engineInstance->GetScreenHeight()) - tmpY;
	//If this is the first time the mouse position is being updated, set prev and mouse values to be the tmp values.
	if (firstUpdate)
	{
		prevMouse.x = mouse.x = tmpX;
		prevMouse.y = mouse.y = tmpY;
		firstUpdate = false;
	}
	//If the mouse position has been updated before, set prev position to current mouse pos, then set current mouse position to the tmp values.
	else
	{
		prevMouse.x = mouse.x;
		prevMouse.y = mouse.y;
		mouse.x = tmpX;
		mouse.y = tmpY;
	}
}
