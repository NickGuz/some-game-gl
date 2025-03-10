#pragma once

#include "Shader.h"
#include "Texture2D.h"

class SpriteRenderer
{
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void draw_sprite(Texture2D& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f),
        glm::vec2 sprite_offset = glm::vec2(1.0f, 1.0f),
        glm::vec2 sprite_size = glm::vec2(1.0f, 1.0f));

    void draw_global_shader(glm::vec2 position, glm::vec2 size, float rotate, float time);

private:
	Shader shader;
	unsigned int quadVAO;
	
	void init_render_data();
};

