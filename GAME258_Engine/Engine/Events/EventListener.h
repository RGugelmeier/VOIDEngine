#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "MouseEventListener.h"
//#include "KeyBoardEventListener.h" Uncomment when KeyboardEventListener is implemented

class EventListener {
public:
	//Purely static class. Cannot be created as an object.
	EventListener(const EventListener&) = delete;
	EventListener(EventListener&&) = delete;
	EventListener& operator=(const EventListener&) = delete;
	EventListener& operator=(EventListener&) = delete;
	EventListener() = delete;

	~EventListener();

	static void Update();
};

#endif // !EVENTLISTENER_H
