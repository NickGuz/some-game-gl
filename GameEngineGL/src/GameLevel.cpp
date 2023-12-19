#include "GameLevel.h"
#include "logger.h"

GameLevel::GameLevel(const CharacterObject& player) : player(player), completed(false) {
}

void GameLevel::init(const rapidjson::Value& background_layer, 
                     const rapidjson::Value& collidable_layer, 
                     const rapidjson::Value& movable_layer,
                     const rapidjson::Value& finish_layer,
                     unsigned int level_width, 
                     unsigned int level_height) {
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
void GameLevel::load(const char* file, unsigned int level_width, unsigned int level_height) {
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

    init(background_layer, collision_layer, movable_layer, finish_layer, level_width, level_height);
}

void GameLevel::update(float delta_time) {
	// handle all events the receiver is subscribed to
	while (receiver.hasNextEvent()) {
		const GameEvent event = receiver.getNextEvent();

		// pass this off to handler function probably, but for now just doing here...
		if (event.type == "LEVEL_END") {
			log_debug("HANDLING LEVEL_END EVENT");
			completed = true;
		}
	}

    // update boxes
    /* for (GameObject& box : bricks) { */
    /*     box.position.x += box.velocity.x * delta_time; */
    /*     box.position.y -= box.velocity.y * delta_time; */
    /* } */

    for (GameObject* box : movable_bricks) {
        box->position.x += box->velocity.x * delta_time;
        box->position.y -= box->velocity.y * delta_time;
    }

	for (Trigger trigger : triggers) {
        for (const GameObject* box: movable_bricks) {
            if (trigger.aabb_collides(*box)) {
                // kind of a stupid roundabout way of doing this.... but i was initially intended to handle the event elsewhere
                emitter.fireEvent("LEVEL_END");
                std::cout << "LEVEL END" << std::endl;
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

	for (Trigger trigger : triggers) {
		//trigger.draw(renderer);
	}
}

bool GameLevel::is_completed() {
	// maybe move trigger checking code in here?
	return completed;
}
