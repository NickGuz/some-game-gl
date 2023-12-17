#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Spritesheet {
    int sprite_width;
    int sprite_height;
    int tex_width;
    int tex_height;

    int tex;

public:
    Spritesheet(int t, int t_w, int t_h, int s_w, int s_h)
    : tex(t), tex_width(t_w), tex_height(t_h), sprite_width(s_w), sprite_height(s_h)
    {}

    void draw_sprite(float pos_x, float pos_y, int frame_index);
};
