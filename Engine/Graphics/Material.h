#pragma once
#include "Program.h"
#include "Texture.h"

namespace nc
{
	struct Material : public Resource
	{
	public:
		Material() {} 
		Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess) :
			ambient{ ambient },
			diffuse{ diffuse },
			specular{ specular },
			shininess{ shininess }
		{}

		bool Load(const std::string& filename, void* data = nullptr) override;

		void Set();
		void SetShader(const std::shared_ptr<Program>& shader) { this->shader = shader; }

		void Bind();
		void SetProgram(Program& program);
		void AddTexture(const std::shared_ptr<Texture>& texture) { textures.push_back(texture); }

	public:
		glm::vec3 ambient = glm::vec3{ 0 };
		glm::vec3 diffuse = glm::vec3{ 1 };
		glm::vec3 specular = glm::vec3{ 1 };
		float shininess = 100.0f;

		std::shared_ptr<Program> shader;
		std::vector<std::shared_ptr<Texture>> textures;
	};
}
