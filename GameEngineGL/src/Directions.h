#pragma once

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT,
	NONE
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	C_FORWARD,
	C_BACKWARD,
	C_LEFT,
	C_RIGHT
};
