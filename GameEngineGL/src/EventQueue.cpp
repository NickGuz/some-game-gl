#include "EventQueue.h"

EventQueue::EventQueue() {

}

void EventQueue::addEvent(const GameEvent& event) {
	events.push_back(event);
}

void EventQueue::subscribe(Receiver receiver, std::string type) {
	if (receivers.count(type) > 0) {
		receivers[type].push_back(receiver);
	}
	else {
		//std::vector<std::reference_wrapper<Receiver>> v;
		std::vector<Receiver> v;
		v.push_back(receiver);
		receivers[type] = v;
	}
}

//void EventQueue::unsubscribe(const Receiver& receiver, std::string type) {
//	for (std::pair<std::string, std::vector<std::reference_wrapper<Receiver>>> r : receivers) {
//		int index = getVecIndex(r.second, receiver);
//		if (index != -1) {
//			receivers[r.first].erase(receivers[r.first].begin() + index);
//		}
//	}
//}
//
//int EventQueue::getVecIndex(std::vector<std::reference_wrapper<Receiver>> v, Receiver& receiver) {
//	//auto it = std::find(v.begin(), v.end(), receiver);
//	for (int i = 0; i < v.size(); i++) {
//		if (v[i].get() == receiver) {
//
//		}
//	}
//	int index = -1;
//
//	if (it != v.end()) {
//		index = it - v.begin();
//	}
//
//	return index;
//}

void EventQueue::update(float deltaT) {
	while (!events.empty()) {
		// Retrieve each event
		const GameEvent& event = events.front();
		events.pop_front();

		// If a receiver has this event type, send it the event
		if (receivers.count(event.type) > 0) {
			for (Receiver& r : receivers[event.type]) {
				r.receive(event);
			}
		}

		// If a receiver is subscribed to all events, send it the event
		if (receivers.count("ALL") > 0) {
			for (Receiver& r : receivers["ALL"]) {
				r.receive(event);
			}
		}
	}
}
