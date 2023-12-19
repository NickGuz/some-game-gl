#include "FontRenderer.h"

FontRenderer::FontRenderer() {}

void FontRenderer::init() {
    gltInit();
    /* if (TTF_Init() < 0) { */
    /*     std::cout << "Failed to initialize SDL TTF: " << SDL_GetError() << std::endl; */
    /* } */

    /* font = TTF_OpenFont("fonts/arial.ttf", FONT_SIZE); */
}

/* void FontRenderer::draw_text(char* text) { */
/*     // generate the text */
/*     SDL_Color color = {255, 255, 255}; */
/*     SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color); */
/*     SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); */
/* } */

GLTtext* FontRenderer::prep_text(std::string text, float scale, float* ret_width, float* ret_height) {
    GLTtext* glt_text = gltCreateText();
    gltSetText(glt_text, text.c_str());

    // return width and height
    float width = gltGetTextWidth(glt_text, scale);
    float height = gltGetTextHeight(glt_text, scale);
    *ret_width = width;
    *ret_height = height;

    return glt_text;
}

void FontRenderer::draw_text(GLTtext* text, float x, float y, float scale) {
    gltBeginDraw();

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(text, x, y, scale);

    gltEndDraw();
    gltDeleteText(text);
}

void FontRenderer::draw_text(std::string text, float x, float y, float scale) {
    // creating text
    GLTtext* glt_text = gltCreateText();
    gltSetText(glt_text, text.c_str());

    // begin text drawing (this for instance calls glUseProgram)
    gltBeginDraw();

    // draw any amount of text between begin and end
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(glt_text, x, y, scale);

    // finish drawing text
    gltEndDraw();

    // deleting text
    gltDeleteText(glt_text);
}

void FontRenderer::terminate() {
    gltTerminate();
    /* TTF_Quit(); */
}

FontRenderer::~FontRenderer() {
    terminate();
}

/*     FT_Library ft; */
/*     if (FT_Init_FreeType(&ft)) { */
/*         std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl; */
/*         return; */
/*     } */

/*     FT_Face face; */
/*     if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face)) { */
/*         std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl; */
/*         return; */
/*     } */

/*     FT_Set_Pixel_Sizes(face, 0, 48); */

/*     if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) { */
/*         std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl; */
/*         return; */
/*     } */

/*     glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction */

/*     for (unsigned char c = 0; c < 128; c++) { */
/*         // load character glyph */
/*         if (FT_Load_Char(face, c, FT_LOAD_RENDER)) { */
/*             std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl; */
/*             continue; */
/*         } */

/*         // generate texture */
/*         unsigned int texture; */
/*         glGenTextures(1, &texture); */
/*         glBindTexture(GL_TEXTURE_2D, texture); */
/*         glTexImage2D( */
/*             GL_TEXTURE_2D, */
/*             0, */
/*             GL_RED, */
/*             face->glyph->bitmap.width, */
/*             face->glyph->bitmap.rows, */
/*             0, */
/*             GL_RED, */
/*             GL_UNSIGNED_BYTE, */
/*             face->glyph->bitmap.buffer */
/*         ); */

/*         // set texture options */
/*         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); */
/*         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); */
/*         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); */
/*         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */

/*         // now store character for later use */
/*         Character character = { */
/*             texture, */
/*             glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), */
/*             glm::ivec2(face->glyph->bitmap_left,  face->glyph->bitmap_top), */
/*             face->glyph->advance.x */
/*         }; */
/*         characters.insert(std::pair<char, Character>(c, character)); */
/*         /1* characters[c] = character; *1/ */
/*     } */

/*     // clear the resources once we're done */
/*     FT_Done_Face(face); */
/*     FT_Done_FreeType(ft); */

/*     // create vbo and vao */
/*     glGenVertexArrays(1, &VAO); */
/*     glGenBuffers(1, &VBO); */
/*     glBindVertexArray(VAO); */
/*     glBindBuffer(GL_ARRAY_BUFFER, VBO); */
/*     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); */
/*     glEnableVertexAttribArray(0); */
/*     glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); */
/*     glBindBuffer(GL_ARRAY_BUFFER, 0); */
/*     glBindVertexArray(0); */
/* } */

/* void FontRenderer::render_text(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color) { */
/*     // activate corresponding render state */
/*     s.use(); */
/*     glUniform3f(glGetUniformLocation(s.id, "textColor"), color.x, color.y, color.z); */
/*     glActiveTexture(GL_TEXTURE0); */
/*     glBindVertexArray(VAO); */

/*     // iterate through all characters */
/*     for (char c : text) { */
/*         Character ch = characters[c]; */

/*         float xpos = x + ch.bearing.x * scale; */ 
/*         float ypos = y - (ch.size.y - ch.bearing.y) * scale; */

/*         float w = ch.size.x * scale; */
/*         float h = ch.size.y * scale; */

/*         // update VBO for each character */
/*         float vertices[6][4] = { */
/*             { xpos,     ypos + h, 0.0f, 0.0f }, */
/*             { xpos,     ypos,     0.0f, 1.0f }, */
/*             { xpos + w, ypos,     1.0f, 1.0f }, */
            
/*             { xpos,     ypos + h, 0.0f, 0.0f }, */
/*             { xpos + w, ypos,     1.0f, 1.0f }, */
/*             { xpos + w, ypos + h, 1.0f, 0.0f } */
/*         }; */

/*         // render glyph texture over quad */
/*         glBindTexture(GL_TEXTURE_2D, ch.texture_id); */

/*         // update content of VBO memory */
/*         glBindBuffer(GL_ARRAY_BUFFER, VBO); */
/*         glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); */
/*         glBindBuffer(GL_ARRAY_BUFFER, 0); */

/*         // render quad */
/*         glDrawArrays(GL_TRIANGLES, 0, 6); */

/*         // now advance cursors for next glyph (note that advance is number of 1/64 pixels) */
/*         x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64) */
/*     } */


/*     glBindVertexArray(0); */
/*     glBindTexture(GL_TEXTURE_2D, 0); */
/* } */
