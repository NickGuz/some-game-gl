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
#include "FontRenderer.h"

const float BOX_SPEED = 120.0f;

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class GameLevel
{
public:
	// level state
	std::vector<GameObject> bricks;
    std::vector<GameObject> collidable_bricks;
    std::vector<GameObject*> movable_bricks;
	std::vector<Trigger> triggers;
	CharacterObject* player;  // this might be a bad way to do it
	Receiver receiver;
	Emitter emitter;
    FontRenderer* font_renderer;

    unsigned int level_width;
    unsigned int level_height;

	GameLevel(CharacterObject* player, FontRenderer* font_renderer, unsigned int level_width, unsigned int level_height);

	// loads level from file
	void load(const char* file);

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
              const rapidjson::Value& finish_layer
              );

	bool completed;
    bool end_timer_active;
    float end_time;
    glm::vec2 player_start_pos;

    void do_collisions();
};

