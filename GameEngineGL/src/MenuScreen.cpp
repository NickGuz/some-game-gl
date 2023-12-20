#include "MenuScreen.h"

MenuScreen::MenuScreen(FontRenderer* font_renderer, unsigned int screen_width, unsigned int screen_height) 
    : font_renderer(font_renderer), screen_width(screen_width), screen_height(screen_height) {}

void MenuScreen::update(float dt) {
    /* while (receiver.hasNextEvent()) { */
    /*     const GameEvent event = receiver.getNextEvent(); */
    /*     if (event.type == "GAME_START") { */

    /*     } */
    /* } */

}

void MenuScreen::draw() {
    float width, height;
    float scale = 2.0f;
    GLTtext* text = font_renderer->prep_text("More Light", scale, &width, &height);
    font_renderer->draw_text(text, 
            (screen_width / 2.0f) - (width / 2.0f), 
            (screen_height / 2.0f) - (height / 2.0f), scale);

    scale = 1.5f;
    GLTtext* desc = font_renderer->prep_text("Press space to continue...", scale, &width, &height);
    font_renderer->draw_text(desc,
            (screen_width / 2.0f) - (width / 2.0f),
            (screen_height / 1.5f), scale);
}

void MenuScreen::end() {

}
