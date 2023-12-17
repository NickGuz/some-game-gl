#include "Spritesheet.h"

void Spritesheet::draw_sprite(float pos_x, float pos_y, int frame_index) {
    const float verts[] = {
        pos_x,                pos_y,
        pos_x + sprite_width, pos_y,
        pos_x + sprite_width, pos_y + sprite_height,
        pos_x,                pos_y + sprite_height
    };

    const float tw = float(sprite_width) / tex_width;
    const float th = float(sprite_height) / tex_height;

    const int num_per_row = tex_width / sprite_width;

    const float tx = (frame_index % num_per_row) * tw;
    const float ty = (frame_index / num_per_row + 1) * th;

    const float tex_verts[] = {
        tx     , ty,
        tx + tw, ty,
        tx + tw, ty + th,
        tx     , ty + th
    };

    // bind the texture, enable the proper arrays

    glVertexPointer(2, GL_FLOAT, 0, verts);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_verts);
    glDrawArrays(GL_QUADS, 0, 4);
}
