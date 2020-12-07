#pragma once

#include "GL/glew.h"

#include "Logger.h"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Utils
{
	struct SVertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
	};

	struct SUniformSettings
	{
		glm::vec4 TessLevels = { 1.0, 1.0f, 1.0f, 1.0f };
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
	};

	bool ReadFile(const std::string& filename, std::string& result);
	bool LoadShader(const std::string path, GLenum shaderType, GLuint& shader);
}