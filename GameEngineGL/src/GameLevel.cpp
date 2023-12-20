#include "GameLevel.h"
#include "logger.h"

GameLevel::GameLevel(CharacterObject* player, FontRenderer* font_renderer, unsigned int level_width, unsigned int level_height) 
    : player(player), font_renderer(font_renderer), level_width(level_width), level_height(level_height), completed(false), end_timer_active(false) {
    player_start_pos = player->position;
}

void GameLevel::init(const rapidjson::Value& background_layer, 
                     const rapidjson::Value& collidable_layer, 
                     const rapidjson::Value& movable_layer,
                     const rapidjson::Value& finish_layer) {
    // calculate dimensions
    unsigned int height = background_layer["height"].GetInt();
    unsigned int width = background_layer["width"].GetInt();
    //float unit_width = level_width / static_cast<float>(width);
    //float unit_height = level_height / static_cast<float>(height);
    float unit_width = 50.0f;
    float unit_height = unit_width;
    std::cout << "unit_width: " << std::to_string(unit_width) << std::endl;
    std::cout << "unit_height: " << std::to_string(unit_height) << std::endl;

    // initialize background tiles
    int i = 0;
    int x = 0;
    int y = 0;
    for (auto& _n : background_layer["data"].GetArray()) {
        int n = _n.GetInt();
        if (n > 1) {
            glm::vec3 color = glm::vec3(0.7f);
            glm::vec2 pos(unit_width * x, unit_height * y);
            glm::vec2 size(unit_width, unit_height);
            bricks.push_back(GameObject(pos, size, ResourceManager::get_texture("pb"), false, false, color));
        }

        i++;
        x = i % width;
        if (x == 0) {
            y++;
        }
    }

    // initialize collidable tiles
    i = 0;
    x = 0;
    y = 0;
    for (auto& _n : collidable_layer["data"].GetArray()) {
        int n = _n.GetInt();
        if (n > 1) {
            glm::vec3 color = glm::vec3(0.0f);
            glm::vec2 pos(unit_width * x, unit_height * y);
            glm::vec2 size(unit_width, unit_height);
            GameObject brick = GameObject(pos, size, ResourceManager::get_texture("pb"), true, false, color);
            bricks.push_back(brick);
            collidable_bricks.push_back(brick);
        }

        i++;
        x = i % width;
        if (x == 0) {
            y++;
        }
    }

    // initialize movable tiles
    i = 0;
    x = 0;
    y = 0;
    for (auto& _n : movable_layer["data"].GetArray()) {
        int n = _n.GetInt();
        if (n > 1) {
            glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
            glm::vec2 pos(unit_width * x, unit_height * y);
            glm::vec2 size(unit_width, unit_height);
            /* GameObject box = GameObject(pos, size, ResourceManager::get_texture("pb"), true, true, color); */
            /* bricks.push_back(box); */
            GameObject* box_ptr = new GameObject(pos, size, ResourceManager::get_texture("pb"), true, true, color);
            movable_bricks.push_back(box_ptr);
        }

        i++;
        x = i % width;
        if (x == 0) {
            y++;
        }
    }

    // initialize finish tiles
    i = 0;
    x = 0;
    y = 0;
    for (auto& _n : finish_layer["data"].GetArray()) {
        int n = _n.GetInt();
        if (n > 1) {
            glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);
            glm::vec2 pos(unit_width * x, unit_height * y);
            glm::vec2 size(unit_width, unit_height);
            bricks.push_back(GameObject(pos, size, ResourceManager::get_texture("pb"), false, false, color));
            triggers.push_back(Trigger(pos, size));
        }

        i++;
        x = i % width;
        if (x == 0) {
            y++;
        }
    }

    receiver.subscribe("LEVEL_END");
}

// load json file now instead
void GameLevel::load(const char* file) {
    // init some variables on a reset
    end_time = 0.0f;
    end_timer_active = false;
    completed = false;
    bricks.clear();
    movable_bricks.clear();
    collidable_bricks.clear();
    triggers.clear();
    player->position = player_start_pos;

    // file is filename, need to get actual file contents first
    std::ifstream t(file);
    std::stringstream buffer;
    buffer << t.rdbuf();

    // 1. parse a json string into dom
    rapidjson::Document document;
    document.Parse(buffer.str().c_str());
    std::cout << "File parsed" << std::endl;

    // get layers
    const rapidjson::Value& layers = document["layers"];
    const rapidjson::Value& background_layer = layers[0]; // not very safe but whatever
    const rapidjson::Value& collision_layer = layers[1];
    const rapidjson::Value& movable_layer = layers[2];
    const rapidjson::Value& finish_layer = layers[3];

    std::cout << background_layer["class"].GetString() << std::endl;
    std::cout << collision_layer["class"].GetString() << std::endl;
    std::cout << movable_layer["class"].GetString() << std::endl;
    std::cout << finish_layer["class"].GetString() << std::endl;

    init(background_layer, collision_layer, movable_layer, finish_layer);
}

void GameLevel::update(float delta_time) {
    // wait 3 seconds on level end
    if (end_timer_active) {
        end_time += delta_time;
        if (end_time > 3.0f) {
            end_timer_active = false;
        }
    }

    player->move(delta_time);

    do_collisions();

    for (GameObject* box : movable_bricks) {
        box->position.x += box->velocity.x * delta_time;
        box->position.y -= box->velocity.y * delta_time;
    }

	for (Trigger trigger : triggers) {
        for (const GameObject* box: movable_bricks) {
            if (trigger.aabb_collides(*box) && !completed) {
                log_info("LEVEL END");
                completed = true;
                end_timer_active = true;
                end_time = delta_time;
            }
        }
	}
}

void GameLevel::draw(SpriteRenderer& renderer) {
	for (GameObject& tile : bricks) {
        tile.draw(renderer);
	}

    for (GameObject* tile : movable_bricks) {
        tile->draw(renderer);
    }

	//for (Trigger trigger : triggers) {
	//	//trigger.draw(renderer);
	//}

    player->draw(renderer);

    // draw text if level completed
    if (completed && end_timer_active) {
        float width, height;
        float scale = 3.0f;
        GLTtext* text = font_renderer->prep_text("You win!", scale, &width, &height);
        font_renderer->draw_text(
                text, 
                (level_width / 2.0f) - (width / 2.0f), 
                (level_height / 2.0f) - (height / 2.0f), 
                scale
        );
    }
}

bool GameLevel::is_completed() {
	// maybe move trigger checking code in here?
	return completed && !end_timer_active;
}

static Direction check_dir(const GameObject& one, const GameObject& two) {
	glm::vec2 distance = one.position - two.position;
	glm::vec2 min_distance = (one.size + two.size) / 2.0f - glm::abs(distance);

	if (min_distance.x > 0 && min_distance.y > 0) {
		if (min_distance.x < min_distance.y) {
			return (distance.x < 0) ? RIGHT : LEFT;
		}
		else {
			return (distance.y < 0) ? UP : DOWN;
		}
	}

	return NONE;
}

// AABB - AABB collision
static Collision check_collision(const GameObject& one, const GameObject& two) {
	Direction dir = check_dir(one, two);
	bool collided = dir != NONE;
	glm::vec2 diff = one.position - two.position;
	
	return std::make_tuple(collided, dir, diff);
}

void GameLevel::do_collisions() {
	for (GameObject& box : bricks) {
		if (box.collidable) {

            // check box-player collisions
			Collision collision = check_collision(*player, box);
			if (std::get<0>(collision)) {
				// collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vec = std::get<2>(collision);

				// horizontal collision
				if (dir == LEFT || dir == RIGHT) {
					player->velocity.x = 0.0f;

					// relocate
					float penetration = (player->size.x + box.size.x) / 2.0f - std::abs(diff_vec.x);
					if (dir == LEFT) {
						player->position.x += penetration;
                    } else {
						player->position.x -= penetration;
                    }
				}
				else {  // vertical collision
					player->velocity.y = 0.0f;

					// relocate
					float penetration = (player->size.y + box.size.y) / 2.0f - std::abs(diff_vec.y);
					if (dir == UP) {
						player->position.y -= penetration;
                    } else {
						player->position.y += penetration;
                    }
				}
			}
        }
    }

    for (GameObject* box : movable_bricks) {
            // check box-player collisions
			Collision collision = check_collision(*player, *box);
			if (std::get<0>(collision)) {
				// collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vec = std::get<2>(collision);

				// horizontal collision
				if (dir == LEFT || dir == RIGHT) {
					player->velocity.x = 0.0f;

					// relocate
					float penetration = (player->size.x + box->size.x) / 2.0f - std::abs(diff_vec.x);
					if (dir == LEFT) {
                        box->velocity.x -= BOX_SPEED;
						player->position.x += penetration;
                    } else {
                        box->velocity.x += BOX_SPEED;
						player->position.x -= penetration;
                    }
				}
				else {  // vertical collision
					player->velocity.y = 0.0f;

					// relocate
					float penetration = (player->size.y + box->size.y) / 2.0f - std::abs(diff_vec.y);
					if (dir == UP) {
                        box->velocity.y -= BOX_SPEED;
						player->position.y -= penetration;
                    } else {
                        box->velocity.y += BOX_SPEED;
						player->position.y += penetration;
                    }
				}
			}

        // check movable-collidable(not movable) collisions
        for (GameObject& collidable : collidable_bricks) {
            Collision collision = check_collision(*box, collidable);
            if (std::get<0>(collision)) {
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vec = std::get<2>(collision);

                // horizontal collision
                if (dir == LEFT || dir == RIGHT) {
                    box->velocity.x = 0.0f;
                    
                    // relocate
                    float penetration = (box->size.x + collidable.size.x) / 2.0f - std::abs(diff_vec.x);
                    penetration *= dir == LEFT ? 1 : -1;
                    box->position.x += penetration;
                } else {
                    box->velocity.y = 0.0f;

                    // relocate
                    float penetration = (box->size.y + collidable.size.y) / 2.0f - std::abs(diff_vec.y);
                    penetration *= dir == UP ? 1 : -1;
                    box->position.y -= penetration;
                }
            }
        }
    }
}
