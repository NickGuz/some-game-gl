#pragma once

#include <queue>
#include <unordered_map>
#include <iostream>

#include "GameEvent.h"
#include "Receiver.h"
#include "EventType.h"

const int MAX_QUEUE_SIZE = 100;

class EventQueue
{
public:
	// Singleton stuff
	static EventQueue& getInstance() {
		static EventQueue instance;
		return instance;
	}

	EventQueue(const EventQueue&) = delete;
	void operator=(const EventQueue&) = delete;

	void addEvent(const	GameEvent event);

	// TODO should probably make all of these receivers const but then can't call the function so idk
	void subscribe(Receiver* receiver, std::string type);

	//void unsubscribe(const Receiver& receiver, std::string type);

	void update(float deltaT);

private:
	EventQueue();

	std::deque<GameEvent> events;
	//std::unordered_map<std::string, std::vector<std::reference_wrapper<Receiver>>> receivers;
	//std::unordered_map<std::string, std::vector<Receiver>> receivers;
	std::unordered_map<std::string, std::vector<Receiver*>> receivers;

	// Associate the receiver and the type
	//void addListener(const Receiver& receiver, std::string type);

	//int getVecIndex(std::vector<std::reference_wrapper<Receiver>> v, const Receiver& receiver);

};

