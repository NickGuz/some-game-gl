#pragma once

#include <string>

#include "EventQueue.h"
#include "GameEvent.h"

class Emitter
{
public:
	Emitter();

	// Emit an event of type eventType with the data packet data
	void fireEvent(std::string type, std::unordered_map<std::string, std::string> data);
	void fireEvent(std::string type);

private:
	EventQueue& eventQueue;
};

