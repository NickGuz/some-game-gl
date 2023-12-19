#pragma once

/* #include <ft2build.h> */
/* #include FT_FREETYPE_H */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define GLT_IMPLEMENTATION
/* #define GLT_MANUAL_VIEWPORT */
#include <gltext.h>

/* #include <SDL2/SDL_ttf.h> */

#include <iostream>
#include <array>
#include <map>

/* #include "Shader.h" */

/* struct Character { */
/*     unsigned int texture_id; */
/*     glm::ivec2   size; */
/*     glm::ivec2   bearing; // offset from baseline to left/top of glyph */
/*     long advance; // offset to advance to next glyph */
/* }; */

const int FONT_SIZE = 48;

class FontRenderer {
public:
    /* std::map<char, Character> characters; */ 
    /* std::array<Character, 128> characters; */
    unsigned int VAO, VBO;
    /* TTF_Font* font; */
    /* SDL_Renderer* renderer; */

    ~FontRenderer();

	/* // Singleton stuff */
	/* static FontRenderer& getInstance() { */
	/* 	static FontRenderer instance; */
	/* 	return instance; */
	/* } */

	/* FontRenderer(const FontRenderer&) = delete; */
	/* void operator=(const FontRenderer&) = delete; */

    void init();
    void terminate();
    /* void render_text(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color); */
    GLTtext* prep_text(std::string text, float scale, float* ret_width, float* ret_height);
    void draw_text(std::string text, float x, float y, float scale);
    void draw_text(GLTtext* text, float x, float y, float scale);

/* private: */
    FontRenderer();
};
