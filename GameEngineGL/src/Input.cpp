#include "Input.h"

Input::Input() {

}

void Input::process(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		emitter.fireEvent("MOVE_FORWARD");
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		emitter.fireEvent("MOVE_BACKWARD");
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		emitter.fireEvent("MOVE_LEFT");
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		emitter.fireEvent("MOVE_RIGHT");
	}

}
