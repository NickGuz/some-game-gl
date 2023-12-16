#include "CharacterObject.h"

CharacterObject::CharacterObject()
	: GameObject(), jumping(false) {}

CharacterObject::CharacterObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, size, sprite, true, glm::vec3(1.0f), velocity), jumping(false) {}

glm::vec2 CharacterObject::move(float deltaT, unsigned int window_width, unsigned int window_height) {
	//position.y -= velocity.y * deltaT;
	position.y -= velocity.y * deltaT;
	position.x += velocity.x * deltaT;

	// check if outside window bounds
	//if (position.x <= 0.0f) {
	//	velocity.x = -velocity.x;
	//	position.x = 0.0f;
	//}
	//else if (position.x + size.x >= window_width) {
	//	velocity.x = -velocity.x;
	//	position.x = window_width - size.x;
	//}

	//// top of the screen
	//if (position.y <= 0.0f) {
	//	velocity.y = 0.0f;
	//	position.y = 0.0f;
	//}

	//// bottom of the screen
	//if (position.y >= window_height - size.y) {
	//	velocity.y = 0.0f;
	//	position.y = window_height - size.y;
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
