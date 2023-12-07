#include "GameLevel.h"
#include "logger.h"

GameLevel::GameLevel(const CharacterObject& player) : player(player) {
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height) {
	// calculate dimensions
	unsigned int height = tile_data.size();
	unsigned int width = tile_data[0].size();
	float unit_width = level_width / static_cast<float>(width);
	float unit_height = level_height / static_cast<float>(height / 2.0f) ;

	// initialize level tiles based on tile_data
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			// check block type from level data (2d level array)
			if (tile_data[y][x] == 1) {  // solid
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::get_texture("block_solid"),
					glm::vec3(0.8f, 0.8f, 0.7f));
				obj.is_solid = true;
				bricks.push_back(obj);
			}
			else if (tile_data[y][x] > 1) {
				glm::vec3 color = glm::vec3(1.0f); // original: white
				if (tile_data[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tile_data[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tile_data[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tile_data[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				bricks.push_back(GameObject(pos, size, ResourceManager::get_texture("block"), color));
			}
		}
	}

	// create triggers
	Trigger trig_level_end(glm::vec2(100.0f, 100.0f), glm::vec2(10.0f, 100.0f));
	triggers.push_back(trig_level_end);

	// subscribe to events
	receiver.subscribe("LEVEL_END");
}

void GameLevel::load(const char* file, unsigned int level_width, unsigned int level_height) {
	// clear old data
	bricks.clear();

	// load from file
	unsigned int tile_code;
	GameLevel level(player);
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tile_data;
	if (fstream) {
		while (std::getline(fstream, line)) { // read each line from level file
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			
			while (sstream >> tile_code) {  // read each word separated by spaces
				row.push_back(tile_code);
			}
			tile_data.push_back(row);
		}

		if (tile_data.size() > 0) {
			init(tile_data, level_width, level_height);
		}
	}
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

	//std::cout << "Player pos: " << std::to_string(player.position.x) << std::endl;
	for (Trigger trigger : triggers) {
		if (trigger.aabb_collides(player)) {

			// kind of a stupid roundabout way of doing this.... but i was initially intended to handle the event elsewhere
			emitter.fireEvent("LEVEL_END");
		}
	}
}

void GameLevel::draw(SpriteRenderer& renderer) {
	for (GameObject& tile : bricks) {
		if (!tile.destroyed) {
			tile.draw(renderer);
		}
	}

	for (Trigger trigger : triggers) {
		//trigger.draw(renderer);
	}
}

bool GameLevel::is_completed() {
	// maybe move trigger checking code in here?
	return completed;
}
