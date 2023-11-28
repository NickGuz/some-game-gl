#pragma once

#include "GameObject.h"

class BallObject : public GameObject
{
public:
	// ball state
	float radius;
	bool stuck;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(float deltaT, unsigned int window_width);

	void reset(glm::vec2 position, glm::vec2 velocity);
};

