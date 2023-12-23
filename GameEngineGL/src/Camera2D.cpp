#include "Camera2D.h"

Camera2D::Camera2D(glm::vec2 position, unsigned int screen_width, unsigned int screen_height, float zoom) :
    Position(position),
    ScreenWidth(screen_width),
    ScreenHeight(screen_height),
    Zoom(zoom),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
    Up(glm::vec3(0.0f, 1.0f, 0.0f))
{
	updateCameraVectors();
	receiver.subscribe("MOVE_FORWARD");
}

Camera2D::Camera2D(float posX, float posY, unsigned int screen_width, unsigned int screen_height, float zoom) :
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
    ScreenWidth(screen_width),
    ScreenHeight(screen_height),
    Zoom(zoom),
    Up(glm::vec3(0.0f, 1.0f, 0.0f))
{
	Position = glm::vec2(posX, posY);
	updateCameraVectors();
	receiver.subscribe("MOVE_FORWARD");
}

glm::mat4 Camera2D::GetViewMatrix() {
	//return glm::lookAt(Position, Position + Front, Up);
    // return glm::ortho(0.0f, (float)ScreenWidth, (float)ScreenHeight, 0.0f, -1.0f, 1.0f) *
    //        glm::lookAt(glm::vec3(Position, 0.0f), glm::vec3(Position, -1.0f), Up);
    return glm::lookAt(glm::vec3(Position, 0.0f), glm::vec3(Position, -1.0f), Up);
}

glm::mat4 Camera2D::GetProjectionMatrix() {
    return glm::ortho(0.0f, (float)ScreenWidth, (float)ScreenHeight, 0.0f, -1.0f, 1.0f);
}

void Camera2D::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	if (direction == C_FORWARD)
		Position.y -= velocity;
	if (direction == C_BACKWARD)
		Position.y += velocity;
	if (direction == C_LEFT)
		Position.x -= velocity;
	if (direction == C_RIGHT)
		Position.x += velocity;

	//Position.y = 0.0f; // keeps user on ground, kinda hacky and might not be good way to do this in the long run
}

void Camera2D::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    // nothing for 2D camera
}

void Camera2D::ProcessMouseScroll(float yoffset) {
	// do nothing for now
}

void Camera2D::updateCameraVectors() {
    // don't need this (yet) for 2d camera?

	// calculate the new Front vector
	// glm::vec3 front;
	// front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	// front.y = sin(glm::radians(Pitch));
	// front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	// Front = glm::normalize(front);

	// // also recalculate the Right and Up vector
	// Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors because their length gets closer to 0 the more you look up or down which results in slower movement
	// Up = glm::normalize(glm::cross(Right, Front));
}

void Camera2D::update(float deltaT) {
	while (receiver.hasNextEvent()) {
		std::cout << "HAS EVENT" << std::endl;
	}

    //Position.x = Player->position.x - (ScreenWidth / 2.0f) + (Player->size.x / 2.0f);
    //Position.y = Player->position.y - (ScreenHeight / 2.0f) + (Player->size.y / 2.0f);
    //std::cout << "Playerpos: " << Player->position.y << ", camerapos: " << Position.y << std::endl;
}

