#pragma once

#include "GameObject.h"

class CharacterObject : public GameObject
{
public:
	// character state
	bool jumping;

	CharacterObject();
	CharacterObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(float deltaT, unsigned int window_width, unsigned int window_height);
	void jump(float deltaT);

	void reset(glm::vec2 position, glm::vec2 velocity);
};

