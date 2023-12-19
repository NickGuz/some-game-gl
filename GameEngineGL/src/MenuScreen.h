#include "FontRenderer.h"

class MenuScreen
{
public:
    FontRenderer* font_renderer;
    unsigned int screen_width;
    unsigned int screen_height;

    MenuScreen(FontRenderer* font_renderer, unsigned int screen_width, unsigned int screen_height);

    void update(float dt);
    void draw();
    void end();
};
