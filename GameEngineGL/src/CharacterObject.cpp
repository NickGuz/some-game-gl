#include "CharacterObject.h"

CharacterObject::CharacterObject()
	: GameObject(), jumping(false) {}

CharacterObject::CharacterObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), jumping(false) {}

glm::vec2 CharacterObject::move(float deltaT, unsigned int window_width, unsigned int window_height) {
	//if (jumping)
	position.y -= velocity.y * deltaT;

	// check if outside window bounds; if so, reverse velocity and restore at correct position
	if (position.x <= 0.0f) {
		velocity.x = -velocity.x;
		position.x = 0.0f;
	}
	else if (position.x + size.x >= window_width) {
		velocity.x = -velocity.x;
		position.x = window_width - size.x;
	}

	// top of the screen
	if (position.y <= 0.0f) {
		//velocity.y = -velocity.y;
		velocity.y = 0.0f;
		position.y = 0.0f;
		//jumping = false;  // for now we'll consider 'landing' ss just hitting the bottom of the screen
	}

	// bottom of the screen
	if (position.y >= window_height - size.y) {
		velocity.y = 0.0f;
		position.y = window_height - size.y;
		//jumping = false;
	}

	// apply gravity
	//if (jumping) {
	//velocity.y -= 500.0f * deltaT;
	//}

	return position;
}

void CharacterObject::jump(float deltaT) {
	//jumping = true;
	//velocity.y = 500.0f;

	// do some logic

	// set to false when landing (but putting here for now so I don't forget)
	//jumping = false;
}

void CharacterObject::reset(glm::vec2 position, glm::vec2 velocity) {
	this->position = position;
	this->velocity = velocity;
	jumping = false;
}
