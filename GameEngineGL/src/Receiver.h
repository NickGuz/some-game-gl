#pragma once

#include <iostream>
#include <queue>

#include "GameEvent.h"

class Receiver
{
public:
	Receiver();

	void destroy();

	// Adds these types of events to this receiver's queue every update
	void subscribe(std::string type);

	// Adds an event to the queue of this receiver
	void receive(const GameEvent& event);

	// Retrieves the next event from the receiver's queue
	const GameEvent& getNextEvent();

	// Looks at the next event in the receiver's queue, but doesn't remove it from the queue
	const GameEvent& peekNextEvent();

	// Returns true if the receiver has any events in its queue
	bool hasNextEvent();

	// Ignores all events this frame
	void ignoreEvents();

private:
	std::deque<GameEvent> q;

};

