#pragma once

#include <iostream>
#include <vector>

#include "GameObject.h"
#include "ResourceManager.h"

class GameLevel
{
public:
	// level state
	std::vector<GameObject> bricks;

	GameLevel() {}

	// loads level from file
	void load(const char* file, unsigned int level_width, unsigned int level_height);

	// render level
	void draw(SpriteRenderer& renderer);

	// check if the level is completed
	bool is_completed();

private:
	// initialize level from tile data
	void init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height);
};

