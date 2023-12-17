#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>
#include <vector>

#include "CharacterObject.h"
#include "ResourceManager.h"
#include "Trigger.h"
#include "Emitter.h"
#include "Receiver.h"

class GameLevel
{
public:
	// level state
	std::vector<GameObject> bricks;
    std::vector<GameObject> collidable_bricks;
    std::vector<GameObject*> movable_bricks;
	std::vector<Trigger> triggers;
	const CharacterObject& player;  // this might be a bad way to do it
	Receiver receiver;
	Emitter emitter;

	GameLevel(const CharacterObject& player);

	// loads level from file
	void load(const char* file, unsigned int level_width, unsigned int level_height);

	void update(float delta_time);

	// render level
	void draw(SpriteRenderer& renderer);

	// check if the level is completed
	bool is_completed();

private:
	// initialize level from tile data
	//void init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height);
    void init(const rapidjson::Value& background_layer, 
              const rapidjson::Value& collidable_layer, 
              const rapidjson::Value& movable_layer,
              const rapidjson::Value& finish_layer,
              unsigned int level_width, 
              unsigned int level_height);
	bool completed;
};

