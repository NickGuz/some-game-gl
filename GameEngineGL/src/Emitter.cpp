#include "Emitter.h"

Emitter::Emitter() : eventQueue(EventQueue::getInstance()) {}

void Emitter::fireEvent(std::string type, std::unordered_map<std::string, std::string> data) {
	eventQueue.addEvent(GameEvent(type, data));
}

void Emitter::fireEvent(std::string type) {
	eventQueue.addEvent(GameEvent(type));
}
