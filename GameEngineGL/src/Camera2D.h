#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Receiver.h"
#include "Directions.h"

// Default camera values
const float SPEED = 300.0f;
const float SENSITIVITY = 0.1f;

class Camera2D
{
public:
    // screen size vals
    unsigned int ScreenWidth;
    unsigned int ScreenHeight;

	// camera attributes
	glm::vec2 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	Camera2D(
        glm::vec2 position = glm::vec2(0.0f, 0.0f), 
        unsigned int screen_width = 800, 
        unsigned screen_height = 600, 
        float zoom = 1.0f
    );

	// constructor with scalar values
	Camera2D(
		float posX,
		float posY,
        unsigned int screen_width = 800,
        unsigned int screen_height = 600,
        float zoom = 1.0f
	);

	// returns the view matrix calculated using euler angles and the lookat matrix
	glm::mat4 GetViewMatrix();

    glm::mat4 GetProjectionMatrix();

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

