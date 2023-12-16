#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture2D.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	// object state
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	float rotation;
	bool is_solid;
	bool destroyed;  // we likely don't need this
	bool collidable;

	// render state
	Texture2D sprite;

	// constructors
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, bool collidable, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	// draw sprite
	virtual void draw(SpriteRenderer& renderer);
};

