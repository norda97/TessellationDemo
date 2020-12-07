#include "Utils.h"


namespace Utils
{
	bool ReadFile(const std::string& filename, std::string& result)
	{
		FILE* file = fopen(filename.c_str(), "rb");

		if (file == NULL)
		{
			return false;
		}

		// Go to end of file
		if (fseek(file, 0, SEEK_END) < 0)
		{
			return false;
		}

		const long size = ftell(file);
		result.resize(size);

		// Go back to beginning of file
		fseek(file, 0, SEEK_SET);

		// Write file to string
		fread(const_cast<char*>(result.data()), 1, size, file);

		fclose(file);

		return true;
	}

	bool LoadShader(const std::string path, GLenum shaderType, GLuint& shader, std::vector<const GLchar*>* pSources)
	{
		std::string shaderCode;
		if (!ReadFile(path, shaderCode))
		{
			shader = GL_FALSE;
			LOG_ERROR("File %s not found", path.c_str());
			return false;
		}

		shader = glCreateShader(shaderType);
		std::vector<const GLchar*> sources;
		if (pSources != nullptr)
		{
			for (auto source : *pSources)
			{
				sources.push_back(source);
			}
		}
		sources.push_back(shaderCode.c_str());
		glShaderSource(shader, sources.size(), sources.data(), NULL);

		glCompileShader(shader);
		GLint compiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE)
		{
			GLint size = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);

			if (size != 0)
			{
				char* errorMsg = new char[size];
				glGetShaderInfoLog(shader, size, NULL, errorMsg);
				LOG_ERROR("Failed to compile %s: %s", path.c_str(), errorMsg);

				delete[] errorMsg;
			}
			glDeleteShader(shader);

			shader = GL_FALSE;
			return false;
		}

		return true;
	}
}