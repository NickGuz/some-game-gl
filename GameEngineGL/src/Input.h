#pragma once

#include <GLFW/glfw3.h>

#include "Emitter.h"

class Input
{
public:
	Input();
	void process(GLFWwindow* window);

private:
	Emitter emitter;
};

