#pragma once

#include "glm/glm.hpp"
#include "GameObject.h"
#include "Directions.h"

class Trigger
{
public:
	glm::vec2 position;
	glm::vec2 size;

	Trigger(glm::vec2 position, glm::vec2 size);

	//void draw(SpriteRenderer& renderer);

	bool aabb_collides(const GameObject& object) const;
};

