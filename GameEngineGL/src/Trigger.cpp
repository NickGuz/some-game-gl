#include "Trigger.h"

Trigger::Trigger(glm::vec2 position, glm::vec2 size) : position(position), size(size) {
}

//static Direction check_dir(const GameObject& one, const GameObject& two) {
//	glm::vec2 distance = one.position - two.position;
//	glm::vec2 min_distance = (one.size + two.size) / 2.0f - glm::abs(distance);
//
//	if (min_distance.x > 0 && min_distance.y > 0) {
//		if (min_distance.x < min_distance.y) {
//			return (distance.x < 0) ? RIGHT : LEFT;
//		}
//		else {
//			return (distance.y < 0) ? UP : DOWN;
//		}
//	}
//
//	return NONE;
//}

bool Trigger::aabb_collides(const GameObject& object) const {
	glm::vec2 distance = this->position - object.position;
	glm::vec2 min_distance = (this->size + object.size) / 2.0f - glm::abs(distance);

	if (min_distance.x > 0 && min_distance.y > 0) {
		return true;
	}
	return false;
}