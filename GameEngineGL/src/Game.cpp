#include "Game.h"
#include "SpriteRenderer.h"
#include "Camera2D.h"

SpriteRenderer* renderer;
CharacterObject* player;
Camera2D* camera;

Game::Game(unsigned int width, unsigned int height)
	: state(GAME_ACTIVE), keys(), width(width), height(height)
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
	ResourceManager::load_shader("../src/shaders/new_shader.vert", "../src/shaders/new_shader.frag", nullptr, "sprite");

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
		static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
	ResourceManager::get_shader("sprite").use().setInt("image", 0);
    ResourceManager::get_shader("sprite").setMat4("view", camera->GetViewMatrix());
	ResourceManager::get_shader("sprite").setMat4("projection", camera->GetProjectionMatrix());

	// set render-specific controls
	renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));

	// load textures
	ResourceManager::load_texture("../textures/background.jpg", false, "background");
	ResourceManager::load_texture("../textures/awesomeface.png", true, "face");
	ResourceManager::load_texture("../textures/block.png", false, "block");
	ResourceManager::load_texture("../textures/block_solid.png", false, "block_solid");
	ResourceManager::load_texture("../textures/paddle.png", true, "paddle");

	// load levels
	GameLevel one; one.load("../levels/one.lvl", width, height);
	GameLevel two; two.load("../levels/two.lvl", width, height);
	GameLevel three; three.load("../levels/three.lvl", width, height);
	GameLevel four; four.load("../levels/four.lvl", width, height);
	levels.push_back(one);
	levels.push_back(two);
	levels.push_back(three);
	levels.push_back(four);
	level = 0;

	// init player
	glm::vec2 player_pos = glm::vec2(
		width / 2.0f - PLAYER_SIZE.x / 2.0f,
		height - PLAYER_SIZE.y * 2.0f
	);
	player = new CharacterObject(player_pos, glm::vec2(50.0f, 50.0f), glm::vec2(0.0f, 0.0f), ResourceManager::get_texture("face"));
}

void Game::update(float deltaT) {
	// update objects
	player->move(deltaT, width, height);
    //camera->update(deltaT);

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
		levels[0].load("../levels/one.lvl", width, height);
	else if (level == 1)
		levels[1].load("../levels/two.lvl", width, height);
	else if (level == 2)
		levels[2].load("../levels/three.lvl", width, height);
	else if (level == 3)
		levels[3].load("../levels/four.lvl", width, height);
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
			if (player->position.x >= 0.0f) {
				player->position.x -= velocity;
			}
            camera->ProcessKeyboard(C_LEFT, deltaT);
		}
		if (keys[GLFW_KEY_D]) {
			if (player->position.x <= width - player->size.x) {
				player->position.x += velocity;
			}
            camera->ProcessKeyboard(C_RIGHT, deltaT);
		}
        if (keys[GLFW_KEY_W]) {
            if (player->position.y <= height - player->size.y) {
                player->position.y -= velocity;
            }
            camera->ProcessKeyboard(C_FORWARD, deltaT);
        }
        if (keys[GLFW_KEY_S]) {
            if (player->position.y >= 0.0f) {
                player->position.y += velocity;
            }
            camera->ProcessKeyboard(C_BACKWARD,  deltaT);
        }
		if (keys[GLFW_KEY_SPACE]) {
			// jump
			//if (!player->jumping)
			player->jump(deltaT);
			//player->position.y -= velocity;
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

// AABB - AABB collision
Collision check_collision(GameObject& one, GameObject& two) {
	// collision x-axis?
	bool collision_x = one.position.x + one.size.x >= two.position.x &&
		two.position.x + two.size.x >= one.position.x;

	// collision y-axis?
	bool collision_y = one.position.y + one.size.y >= two.position.y &&
		two.position.y + two.size.y >= one.position.y;

	glm::vec2 newpos(two.position.x, two.position.y - two.size.y / 2.0f);
	// difference vector between the 2 objects
	//glm::vec2 diff = one.position - two.position;
	glm::vec2 diff = one.position - newpos;

	bool collided = collision_x && collision_y;
	
	return std::make_tuple(collided, vector_direction(diff), diff);
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
		if (!box.destroyed) {

			// Collision collision = check_collision(*player, box);
			// if (std::get<0>(collision)) {
			// 	// collision resolution
			// 	Direction dir = std::get<1>(collision);
			// 	glm::vec2 diff_vec = std::get<2>(collision);

			// 	// horizontal collision
			// 	if (dir == LEFT || dir == RIGHT) {
			// 		player->velocity.x = 0.0f;

			// 		// relocate
			// 		float penetration = (player->size.x / 2.0f) - std::abs(diff_vec.x);
			// 		if (dir == LEFT)
			// 			player->position.x += penetration;
			// 		else
			// 			player->position.x -= penetration;
			// 	}
			// 	else {  // vertical collision
			// 		player->velocity.y = 0.0f;

			// 		// relocate
			// 		float penetration = (player->size.y / 2.0f) - std::abs(diff_vec.y);
			// 		std::cout << "penetration: " << std::to_string(penetration) << std::endl;
			// 		if (dir == UP)
			// 			player->position.y -= penetration;
			// 		else
			// 			player->position.y += penetration;
			// 	}
			// }

			//collision _collision = check_collision(*ball, box);
			//if (std::get<0>(_collision)) {
			//	// destroy block if not solid
			//	if (!box.is_solid)
			//		box.destroyed = true;

			//	// collision resolution
			//	Direction dir = std::get<1>(_collision);
			//	glm::vec2 diff_vector = std::get<2>(_collision);

			//	// horizontal collision
			//	if (dir == LEFT || dir == RIGHT) {
			//		ball->velocity.x = -ball->velocity.x; // reverse horizontal velocity

			//		// relocate
			//		float penetration = ball->radius - std::abs(diff_vector.x);
			//		if (dir == LEFT)
			//			ball->position.x += penetration; // move ball to right
			//		else
			//			ball->position.x -= penetration; // move ball to left
			//	}
			//	else { // vertical collision
			//		ball->velocity.y = -ball->velocity.y; // reverse vertical velocity

			//		// relocate
			//		float penetration = ball->radius - std::abs(diff_vector.y);
			//		if (dir == UP)
			//			ball->position.y -= penetration; // move ball back up
			//		else
			//			ball->position.y += penetration; // move ball back down
			//	}
			//}
		}
	}

	// check collisions for player pad (unless stuck)
	//collision result = check_collision(*ball, *player);
	//if (!ball->stuck && std::get<0>(result)) {
	//	// check where it hit the board, and change velocity based on where it hit the board
	//	float center_board = player->position.x + player->size.x / 2.0f;
	//	float distance = (ball->position.x + ball->radius) - center_board;
	//	float percentage = distance / (player->size.x / 2.0f);

	//	// then move accordingly
	//	float strength = 2.0f;
	//	glm::vec2 old_velocity = ball->velocity;
	//	ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
	//	ball->velocity = glm::normalize(ball->velocity) * glm::length(old_velocity);

	//	// fix sticky paddle
	//	ball->velocity.y = -1.0f * abs(ball->velocity.y);
	//}
}

