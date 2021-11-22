#include "Program.h"

namespace nc
{
	Program::Program()
	{
		program = glCreateProgram();
	}

	Program::~Program()
	{
		if (program != 0)
		{
			// delete program
			glDeleteProgram(program);
		}
	}

	bool Program::Load(const std::string& name, void* null)
	{
		return true;
	}

	void Program::AddShader(const std::shared_ptr<Shader>& shader)
	{
		shaders.push_back(shader);

		glAttachShader(program, shader->shader);
	}

	void Program::Link()
	{
		if (linked) return;

		// link shader programs
		glLinkProgram(program);

		// check program link status
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);

			if (length > 0)
			{
				std::string infoLog(length, ' ');
				glGetProgramInfoLog(program, length, &length, &infoLog[0]);

				SDL_Log("Error: Failed to link program.");
				SDL_Log("Program Info: %s", infoLog.c_str());
			}

			glDeleteProgram(program);
			program = 0;
		}
		else
		{
			linked = true;
		}
	}

	void Program::Use()
	{
		glUseProgram(program);
	}

	void Program::SetUniform(const std::string& name, float x, float y, float z)
	{
		GLint uniform = GetUniform(name);
		glUniform3f(uniform, x, y, z);
	}

	void Program::SetUniform(const std::string& name, const glm::vec2& v2)
	{
		GLint uniform = GetUniform(name);
		glUniform2f(uniform, v2.x, v2.y);
	}

	void Program::SetUniform(const std::string& name, const glm::vec3& v3)
	{
		GLint uniform = GetUniform(name);
		glUniform3f(uniform, v3.x, v3.y, v3.z);
	}

	void Program::SetUniform(const std::string& name, const glm::vec4& v4)
	{
		GLint uniform = GetUniform(name);
		glUniform4f(uniform, v4.x, v4.y, v4.z, v4.w);
	}

	void Program::SetUniform(const std::string& name, const glm::mat4& mx4)
	{
		GLint uniform = GetUniform(name);
		glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(mx4));
	}

	void Program::SetUniform(const std::string& name, const glm::mat3& mx3)
	{
		GLint uniform = GetUniform(name);
		glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(mx3));
	}

	void Program::SetUniform(const std::string& name, float value)
	{
		GLint uniform = GetUniform(name);
		glUniform1f(uniform, value); 
	}

	void Program::SetUniform(const std::string& name, int value)
	{
		GLint uniform = GetUniform(name);
		glUniform1i(uniform, value);
	}

	void Program::SetUniform(const std::string& name, bool value)
	{
		GLint uniform = GetUniform(name);
		glUniform1i(uniform, value);
	}

	void Program::SetUniform(const std::string& name, GLuint value)
	{
		GLint uniform = GetUniform(name);
		glUniform1ui(uniform, value);
	}

	GLint Program::GetUniform(const std::string& name)
	{
		// find uniform in map
		auto uniform = uniforms.find(name);
		if (uniform == uniforms.end())
		{
			// add uniform to map
			GLint uniform = glGetUniformLocation(program, name.c_str());
			if (uniform == -1)
			{
				SDL_Log("Could not find uniform: %s", name.c_str());
			}
			uniforms[name] = uniform;
		}

		return uniforms[name];
	}
}