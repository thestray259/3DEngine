#include "Shader.h"
#include "Core/FileSystem.h" 

namespace nc
{
	Shader::~Shader()
	{
		if (shader != 0)
		{
			// delete shader
			glDeleteShader(shader);
		}
	}

	bool Shader::Load(const std::string& name, void* data)
	{
		// get shader source from file
		std::string source;
		bool success = ReadFileToString(name, source);
		if (success == false)
		{
			SDL_Log("Error: Failed to open file (%s).", name.c_str());
		}

		// create shader
		GLuint shaderType = static_cast<GLuint>(reinterpret_cast<std::uintptr_t>(data));
		shader = glCreateShader(shaderType);

		// compile shader
		const char* source_c = source.c_str(); // <get the c_str of the source string>
		glShaderSource(shader, 1, &source_c, NULL);
		//< compile the shader https ://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCompileShader.xhtml>
		glCompileShader(shader); 

		// check shader compilation status
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			// display shader error
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			if (length > 0)
			{
				std::string infoLog(length, ' ');
				glGetShaderInfoLog(shader, length, &length, &infoLog[0]);
				SDL_Log("Error: Failed to compile shader.");
				SDL_Log("Shader Info: %s", infoLog.c_str());
			}

			// delete shader
			glDeleteShader(shader);
			shader = 0;
		}
		return true;
	}

}