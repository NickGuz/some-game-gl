#include "Shader.h"

void Shader::compile(const char* vertex_source, const char* fragment_source, const char* geometry_source) {
	unsigned int s_vertex, s_fragment, g_shader;

	// vertex shader
	s_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(s_vertex, 1, &vertex_source, NULL);
	glCompileShader(s_vertex);
	checkCompileErrors(s_vertex, "VERTEX");

	// fragment shader
	s_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(s_fragment, 1, &fragment_source, NULL);
	glCompileShader(s_fragment);
	checkCompileErrors(s_fragment, "FRAGMENT");

	// if geometry shader source code is given, also compile geometry shader
	if (geometry_source != nullptr) {
		g_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(g_shader, 1, &geometry_source, NULL);
		glCompileShader(g_shader);
		checkCompileErrors(g_shader, "GEOMETRY");
	}

	// shader program
	this->id = glCreateProgram();
	glAttachShader(this->id, s_vertex);
	glAttachShader(this->id, s_fragment);
	if (geometry_source != nullptr)
		glAttachShader(this->id, g_shader);
	glLinkProgram(this->id);
	checkCompileErrors(this->id, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(s_vertex);
	glDeleteShader(s_fragment);
	if (geometry_source != nullptr)
		glDeleteShader(g_shader);
}

Shader& Shader::use() {
	glUseProgram(this->id);
	return *this;
}

void Shader::deleteProgram() {
	glDeleteProgram(id);
}

void Shader::setBool(const std::string& name, bool value, bool use_shader) {
	if (use_shader)
		this->use();
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value, bool use_shader) {
	if (use_shader)
		this->use();
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value, bool use_shader) {
	if (use_shader)
		this->use();
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, float x, float y, float z, bool use_shader) {
	if (use_shader)
		this->use();
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value, bool use_shader) {
	if (use_shader)
		this->use();
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec2f(const std::string& name, float x, float y, bool use_shader) {
    if (use_shader)
        this->use();
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec2fv(const std::string& name, const glm::vec2& value, bool use_shader) {
    if (use_shader)
        this->use();
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat, bool use_shader) {
	if (use_shader)
		this->use();
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------- -- " << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR or type: " << type << "\n" << infoLog << "\n -- ------------------------------------- -- " << std::endl;
		}
	}
}
