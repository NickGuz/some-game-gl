#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Texture2D.h"
#include "Shader.h"

class ResourceManager
{
public:
	// resource storage
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. if gShaderFile is not nullptr, it also loads a geometry shader
	static Shader load_shader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file, std::string name);

	// retrieves a stored shader
	static Shader& get_shader(std::string name);

	// loads (and generates) a texture from file
	static Texture2D load_texture(const char* file, bool alpha, std::string name);

	// retrieves a stored texture
	static Texture2D& get_texture(std::string name);

	// properly de-allocates all loaded resources
	static void clear();

private:
	// private constructor, that is we do not want any actual resource manager objects. its members and functions should be publically available (static)
	ResourceManager() {}

	// loads and generates a shader from file
	static Shader load_shader_from_file(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file = nullptr);

	// loads a single texture from file
	static Texture2D load_texture_from_file(const char* file, bool alpha);
};

