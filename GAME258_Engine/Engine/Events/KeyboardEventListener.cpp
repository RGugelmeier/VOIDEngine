#include "KeyboardEventListener.h"
//CoreEngine needs to be included in the cpp to avoid circular dependencies. This goes hand in hand with the forward declaration in MouseEventListener.h.
#include "../Core/CoreEngine.h"

//Redeclare and define static members.
CoreEngine* KeyboardEventListener::engineInstance = nullptr;
bool KeyboardEventListener::firstUpdate = true;

KeyboardEventListener::~KeyboardEventListener()
{
	engineInstance = nullptr;
}

void KeyboardEventListener::RegisterEngineObject(CoreEngine* engine_)
{
	engineInstance = engine_;
}

void KeyboardEventListener::Update(SDL_Event e_)
{
	if (e_.type == SDL_KEYDOWN)
	{
		engineInstance->NotifyOfKeyboardPress(e_.key.keysym.sym);
	}
	else if (e_.type == SDL_KEYUP)
	{
		engineInstance->NotifyOfKeyboardRelease(e_.key.keysym.sym);
	}
}

void KeyboardEventListener::NotifyOfButtonPressed(SDL_Keycode e_)
{
	engineInstance->NotifyOfKeyboardPress(e_);
}