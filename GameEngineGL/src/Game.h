#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>

#include "ResourceManager.h"
#include "GameLevel.h"
#include "CharacterObject.h"
#include "BallObject.h"
#include "Directions.h"
#include "Receiver.h"
#include "MenuScreen.h"

// Initial size of the player
const glm::vec2 PLAYER_SIZE(60.0f, 60.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(300.0f);

// Initial velocity of the ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game
{
public:
	// game state
	GameState state;
	bool keys[1024];
	unsigned int width, height;
	std::vector<GameLevel> levels;
	unsigned int level;
	Receiver receiver;

    bool closing;

	// keep a pointer to the window
	GLFWwindow* window;
	
	// constructor/destructor
	Game(unsigned int width, unsigned int height);
	~Game();

	// initialize game state (load all shaders/textures/levels)
	void init();

	// game loop
	void processInput(float deltaT);
	void update(float deltaT);
	void render();

	void reset_level();
    void reset_all_levels();
	void reset_player();

private:
    float time_elapsed;
};

