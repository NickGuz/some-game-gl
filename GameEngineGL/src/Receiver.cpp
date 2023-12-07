#include "Receiver.h"
#include "EventQueue.h"

Receiver::Receiver() {

}

void Receiver::destroy() {
	//EventQueue::getInstance().unsubscribe(this);
}

void Receiver::subscribe(std::string type) {
	EventQueue::getInstance().subscribe(this, type);
	q.clear();
}

void Receiver::receive(GameEvent event) {
	q.push_back(event);
}

const GameEvent Receiver::getNextEvent() {
	const GameEvent e = q.front();
	q.pop_front();
	return e;
}

const GameEvent& Receiver::peekNextEvent() {
	return q.front();
}

bool Receiver::hasNextEvent() {
	return !q.empty();
}

void Receiver::ignoreEvents() {
	q.clear();
}
