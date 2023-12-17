#include "Game.h"
#include "SpriteRenderer.h"
#include "Camera2D.h"
#include "logger.h"

SpriteRenderer* renderer;
CharacterObject* player;
Camera2D* camera;

Game::Game(unsigned int width, unsigned int height)
	: width(width), height(height), state(GAME_ACTIVE), keys()
{
	
}

Game::~Game() {
	delete renderer;
	delete player;
}

void Game::init() {
    // init camera
    camera = new Camera2D();
	// load shaders
	ResourceManager::load_shader("src/shaders/new_shader.vert", "src/shaders/new_shader.frag", nullptr, "sprite");

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
		static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
	ResourceManager::get_shader("sprite").use().setInt("image", 0);
    ResourceManager::get_shader("sprite").setMat4("view", camera->GetViewMatrix());
	ResourceManager::get_shader("sprite").setMat4("projection", camera->GetProjectionMatrix());

	// set render-specific controls
	renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));

	// load textures
	ResourceManager::load_texture("textures/background.jpg", false, "background");
	/* ResourceManager::load_texture("textures/awesomeface.png", true, "face"); */
    ResourceManager::load_texture("textures/face.png", true, "face");
	ResourceManager::load_texture("textures/block.png", false, "block");
	ResourceManager::load_texture("textures/block_solid.png", false, "block_solid");
	ResourceManager::load_texture("textures/paddle.png", true, "paddle");
	ResourceManager::load_texture("textures/container.jpg", false, "desert");
    ResourceManager::load_texture("textures/white_square.png", false, "pb");
    ResourceManager::load_texture("textures/empty-block.png", false, "eb");

	// init player
	glm::vec2 player_pos = glm::vec2(
		width / 2.0f - PLAYER_SIZE.x / 2.0f,
		height - PLAYER_SIZE.y * 2.0f
	);
	player = new CharacterObject(player_pos, glm::vec2(50.0f, 50.0f), glm::vec2(0.0f, 0.0f), ResourceManager::get_texture("face"));
    camera->Player = player;

	// load levels
	GameLevel one(*player); one.load("levels/lvl1.json", width, height);
	/* GameLevel two(*player); two.load("levels/two.lvl", width, height); */
	/* GameLevel three(*player); three.load("levels/three.lvl", width, height); */
	/* GameLevel four(*player); four.load("levels/four.lvl", width, height); */
	levels.push_back(one);
	/* levels.push_back(two); */
	/* levels.push_back(three); */
	/* levels.push_back(four); */
	level = 0;

	// create receiver for events
	//receiver.subscribe("LEVEL_END");
}

void Game::update(float deltaT) {
	// should this be here?
	EventQueue::getInstance().update(deltaT);

	//while (receiver.hasNextEvent()) {
	//	const GameEvent event = receiver.getNextEvent();

	//	// pass this off to handler function probably, but for now just doing here...
	//	if (event.type == "LEVEL_END") {
	//		log_debug("HANDLING LEVEL_END EVENT");
	//	}
	//}
	// update objects
    // TODO we probably want to check for collisions BEFORE moving the player
	player->move(deltaT, width, height);
    camera->update(deltaT);

	// update the level? 
	levels[level].update(deltaT);
	if (levels[level].is_completed()) {
        std::cout << "Level completed" << std::endl;
		glfwSetWindowShouldClose(window, true);
	}

    // update shader (camera) position
    // don't know if this is right
    ResourceManager::get_shader("sprite").use();
    ResourceManager::get_shader("sprite").setMat4("view", camera->GetViewMatrix());
    ResourceManager::get_shader("sprite").setMat4("projection", camera->GetProjectionMatrix());

	// check for collisions
	do_collisions();

	// check loss condition
}

void Game::reset_level() {
	if (level == 0)
		levels[0].load("levels/lvl1.json", width, height);
	/* else if (level == 1) */
	/* 	levels[1].load("levels/two.lvl", width, height); */
	/* else if (level == 2) */
	/* 	levels[2].load("levels/three.lvl", width, height); */
	/* else if (level == 3) */
	/* 	levels[3].load("levels/four.lvl", width, height); */
}

void Game::reset_player() {
	// reset player/ball stats
	player->size = PLAYER_SIZE;
	player->position = glm::vec2(width / 2.0f - PLAYER_SIZE.x / 2.0f, height - PLAYER_SIZE.y);
}

void Game::processInput(float deltaT) {
	if (state == GAME_ACTIVE) {
		float velocity = PLAYER_VELOCITY * deltaT;

		// move player
		if (keys[GLFW_KEY_A]) {
            player->position.x -= velocity;
		}
		if (keys[GLFW_KEY_D]) {
            player->position.x += velocity;
		}
        if (keys[GLFW_KEY_W]) {
            player->position.y -= velocity;
        }
        if (keys[GLFW_KEY_S]) {
            player->position.y += velocity;
        }
		if (keys[GLFW_KEY_SPACE]) {
			player->jump(deltaT);
		}
	}
}

void Game::render() {
	if (state == GAME_ACTIVE) {
		// draw background
		renderer->draw_sprite(ResourceManager::get_texture("background"),
			glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);

		// draw level
		levels[level].draw(*renderer);

		// draw player
		player->draw(*renderer);
	}
}

Direction vector_direction(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),  // up
		glm::vec2(1.0f, 0.0f),  // right
		glm::vec2(0.0f, -1.0f), // down
		glm::vec2(-1.0f, 0.0f)  // left
	};

	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++) {
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}

	return (Direction)best_match;
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

// AABB - Circle collision
Collision check_collision(BallObject& circle, GameObject& obj) {
	// get center point circle first
	glm::vec2 center(circle.position + circle.radius);

	// calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(obj.size.x / 2.0f, obj.size.y / 2.0f);
	glm::vec2 aabb_center(
		obj.position.x + aabb_half_extents.x,
		obj.position.y + aabb_half_extents.y
	);
    
	// get difference bector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	// add clamped value to aabb_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;

	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;
	//return glm::length(difference) < circle.radius;

	if (glm::length(difference) < circle.radius)
		return std::make_tuple(true, vector_direction(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Game::do_collisions() {
	for (GameObject& box : levels[level].bricks) {
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

    for (GameObject* box : levels[level].movable_bricks) {
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
        for (GameObject& collidable : levels[level].collidable_bricks) {
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

