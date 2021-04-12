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
		//Do stuff depending on what event happens. In this case, if any mouse event happens, run MouseEventListener's update with whatever event happens as the parameter.
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
