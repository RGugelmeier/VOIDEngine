#include "EventListener.h"
#include "../Core/CoreEngine.h"

EventListener::~EventListener()
{
}

void EventListener::Update()
{
	SDL_Event sdlEvent;
	//While an event is happening.
	while (SDL_PollEvent(&sdlEvent))
	{
		//If the event is the X button is clicked. Exit the program.
		if (sdlEvent.type == SDL_QUIT)
		{
			CoreEngine::GetInstance()->Exit();
		}

		//If the event is a keyboard button press, call the keyboard event listener's update to check what button was pressed.
		if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP)
		{
			if (sdlEvent.key.repeat == 0)
			{
				KeyboardEventListener::Update(sdlEvent);
			}
		}

		//If the event is not the X button nor keyboard button press.
		switch (sdlEvent.type)
		{
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEWHEEL:
				MouseEventListener::Update(sdlEvent);
				break;
			default:
				break;
		}
	 }
}
