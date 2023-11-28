#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the program ID
	unsigned int id;

	Shader() {}

	// use/activate the shader
	Shader& use();
	void compile(const char* vertex_source, const char* fragment_source, const char* geometry_source = nullptr);
	void deleteProgram();

	// utility uniform functions
	void setBool(const std::string& name, bool value, bool use_shader = false);
	void setInt(const std::string& name, int value, bool use_shader = false);
	void setFloat(const std::string& name, float value, bool use_shader = false);
	void setVec3(const std::string& name, float x, float y, float z, bool use_shader = false);
	void setVec3(const std::string& name, const glm::vec3& value, bool use_shader = false);
	void setMat4(const std::string& name, const glm::mat4& value, bool use_shader = false);

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
