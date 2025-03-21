#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Receiver.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

class Camera
{
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// euler angles
	float Yaw;
	float Pitch;

	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH
	);

	// constructor with scalar values
	Camera(
		float posX,
		float posY,
		float posZ,
		float upX,
		float upY,
		float upZ,
		float yaw,
		float pitch
	);

	// returns the view matrix calculated using euler angles and the lookat matrix
	glm::mat4 GetViewMatrix();

	// processes input received from any keyboard-like input system
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// processes input received from a mouse input system
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// processes input received from a mouse scroll-wheel event
	void ProcessMouseScroll(float yoffset);

	void update(float deltaT);

private:
	// calculates the front vector from the Camera's (updated) euler angles
	void updateCameraVectors();

	Receiver receiver;
};

