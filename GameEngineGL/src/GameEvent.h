#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>

class GameEvent
{
public:
	std::string type;
	std::unordered_map<std::string, std::string> data;
	long long time;

	GameEvent(std::string _type) {
		type = _type;
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();  // not sure yet if this matters
	}

	GameEvent(std::string _type, std::unordered_map<std::string, std::string> _data) {
		data = _data;
		type = _type;
		time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();  // not sure yet if this matters
	}

	// Checks the type of the Event
	bool isType(std::string _type) {
		return type == _type;
	}

	// Returns this Event as a string
	std::string toString() {
		return type + ": @" + std::to_string(time);
	}

};
