#include "SpriteRenderer.h"
#include "logger.h"

SpriteRenderer::SpriteRenderer(Shader& shader) {
	this->shader = shader;
	this->init_render_data();
}

SpriteRenderer::~SpriteRenderer() {
    log_info("Deleting spriterenderer");
    log_info("quadVAO: " + std::to_string(this->quadVAO));
	glDeleteVertexArrays(1, &this->quadVAO);
    log_info("Deleted spriterenderer");
}

/* void SpriteRenderer::draw_sprite(Texture2D& texture, glm::vec2 position, */
/* 	glm::vec2 size, float rotate, glm::vec3 color, glm::vec2 sprite_offset, glm::vec2 sprite_size) */
/* { */
/* 	// prepare transformations */
/* 	this->shader.use(); */
/* 	glm::mat4 model = glm::mat4(1.0f); */
/* 	model = glm::translate(model, glm::vec3(position, 0.0f)); */

/* 	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); */
/* 	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); */
/* 	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); */

/* 	model = glm::scale(model, glm::vec3(size, 1.0f)); */

/* 	this->shader.setMat4("model", model); */
/* 	this->shader.setVec3("spriteColor", color); */

/* 	glActiveTexture(GL_TEXTURE0); */
/* 	texture.bind(); */

/* 	glBindVertexArray(this->quadVAO); */
/* 	glDrawArrays(GL_TRIANGLES, 0, 6); */
/* 	glBindVertexArray(0); */
/* } */

void SpriteRenderer::draw_sprite(Texture2D& texture, glm::vec2 position,
	glm::vec2 size, float rotate, glm::vec3 color, glm::vec2 sprite_offset, glm::vec2 sprite_size)
{
	// prepare transformations
	this->shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.setMat4("model", model);
	this->shader.setVec3("spriteColor", color);

    // set texture coordinates based on sprite_offset and sprite_size
    this->shader.setVec2fv("spriteOffset", sprite_offset);
    this->shader.setVec2fv("spriteSize", sprite_size);
    //this->shader.setVec2fv("spriteSize", glm::vec2(0.5f, 0.5f));

	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::draw_global_shader(glm::vec2 position, glm::vec2 size, float rotate, float time) {
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader.setMat4("model", model);
    this->shader.setFloat("time", time);
    this->shader.setVec2fv("resolution", size);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::init_render_data() {
	// configure VAO/VBO
	unsigned int VBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

