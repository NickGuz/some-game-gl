#include "Game.h"
#include "SpriteRenderer.h"
#include "Camera2D.h"
#include "logger.h"
#include "SoundEngine.h"

SpriteRenderer *renderer;
CharacterObject *player;
Camera2D *camera;
FontRenderer *font_renderer;
SpriteRenderer *shader_renderer;
MenuScreen *title_screen;
SoundEngine *sound_engine;

Game::Game(unsigned int width, unsigned int height)
	: width(width), height(height), state(GAME_MENU), keys()
{

}

Game::~Game() {
	delete renderer;
    delete shader_renderer;
	delete player;
    delete camera;
    delete font_renderer;
    delete title_screen;
}

void Game::init() {
    // init camera
    camera = new Camera2D(0.0f, 0.0f, width, height, 0.9f);

    // load shaders
    ResourceManager::load_shader("src/shaders/new_shader.vert", "src/shaders/new_shader.frag", nullptr, "sprite");
    ResourceManager::load_shader("src/shaders/glyph.vert", "src/shaders/glyph.frag", nullptr, "glyph");
    ResourceManager::load_shader("src/shaders/new_shader2.vert", "src/shaders/level_restart.frag", nullptr, "level_restart");

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
        static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::get_shader("sprite").use().setInt("image", 0);
    ResourceManager::get_shader("sprite").setMat4("view", camera->GetViewMatrix());
    ResourceManager::get_shader("sprite").setMat4("projection", camera->GetProjectionMatrix());

    //ResourceManager::get_shader("level_restart").use().setMat4("view", camera->GetViewMatrix());
    //ResourceManager::get_shader("level_restart").setMat4("projection", camera->GetProjectionMatrix());

    // set render-specific controls
    renderer = new SpriteRenderer(ResourceManager::get_shader("sprite"));
    //shader_renderer = new SpriteRenderer(ResourceManager::get_shader("level_restart"));

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

    font_renderer = new FontRenderer();
    font_renderer->init();

    // init player
    glm::vec2 player_pos = glm::vec2(
        width / 2.0f - PLAYER_SIZE.x / 2.0f,
        height - PLAYER_SIZE.y * 2.0f
    );
    player = new CharacterObject(player_pos, glm::vec2(50.0f, 50.0f), glm::vec2(0.0f, 0.0f), ResourceManager::get_texture("face"));
    camera->Player = player;

    // load menu screens
    title_screen = new MenuScreen(font_renderer, width, height);

    // init sound
    sound_engine = new SoundEngine();

    // load levels
    GameLevel one(player, font_renderer, width, height, sound_engine); one.load("levels/lvl1.json");
    GameLevel two(player, font_renderer, width, height, sound_engine); two.load("levels/lvl2.json");
    /* GameLevel three(*player); three.load("levels/three.lvl", width, height); */
    /* GameLevel four(*player); four.load("levels/four.lvl", width, height); */
    levels.push_back(one);
    levels.push_back(two);
    /* levels.push_back(three); */
    /* levels.push_back(four); */
    level = 0;

    // create receiver for events
    //receiver.subscribe("LEVEL_END");

    // play music
    sound_engine->set_volume(0.0f);
    sound_engine->play_sound("audio/breakout.mp3", true);
}

void Game::update(float deltaT) {
    /* time_elapsed += deltaT; */

    EventQueue::getInstance().update(deltaT);

    switch (state) {
    case GAME_ACTIVE: {
        // update the level? 
        levels[level].update(deltaT);
        if (levels[level].is_completed()) {
            log_info("Level completed");
            /* sound_engine->play2D("audio/finish.wav"); */
            sound_engine->play_sound("audio/finish.wav");
            level++;
            if (level >= levels.size()) {
                state = GAME_MENU;
                level = 0;
                reset_level();
            }
        }
        camera->update(deltaT);
  
        // update shader (camera) position
        // don't know if this is right
        ResourceManager::get_shader("sprite").use();
        ResourceManager::get_shader("sprite").setMat4("view", camera->GetViewMatrix());
        ResourceManager::get_shader("sprite").setMat4("projection", camera->GetProjectionMatrix());

        // TODO i doubt this works
        /* ResourceManager::get_shader("level_end").use(); */
        /* ResourceManager::get_shader("level_end").setMat4("view", camera->GetViewMatrix()); */
        /* ResourceManager::get_shader("level_end").setMat4("projection", camera->GetProjectionMatrix()); */
        /* ResourceManager::get_shader("level_end").setVec2f("resolution", width, height); */
        /* ResourceManager::get_shader("level_end").setFloat("time", time_elapsed); */
        break;
    }
    case GAME_MENU: {
        title_screen->update(deltaT);
        break;
    }
    case GAME_WIN:
        break;
    }
}

void Game::reset_level() {
	if (level == 0)
		levels[0].load("levels/lvl1.json");
	else if (level == 1)
		levels[1].load("levels/lvl2.json");
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
    switch (state) {
    case GAME_ACTIVE: {
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
        if (keys[GLFW_KEY_R]) {
            reset_level();
        }
        break;
    }
    case GAME_MENU: {
        if (keys[GLFW_KEY_SPACE]) {
            /* title_screen->end(); */
            log_info("Space");
            state = GAME_ACTIVE;
        }
        break;
    }
    case GAME_WIN:
        break;
    }
}

void Game::render() {
    switch (state) {
    case GAME_ACTIVE:
        // draw background
        renderer->draw_sprite(ResourceManager::get_texture("background"),
                glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);

        // draw level
        levels[level].draw(*renderer);

        // draw shader
        //shader_renderer->draw_global_shader(glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f, glfwGetTime());
        break;

    case GAME_MENU:
        // draw main menu screen
        title_screen->draw();
        break;

    case GAME_WIN:
        break;
    }
}

