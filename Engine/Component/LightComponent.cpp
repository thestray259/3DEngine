#include "LightComponent.h"
#include "Engine.h"

namespace nc
{
	void LightComponent::Update()
	{
		glm::vec4 position{ 1 };

		// transform the light position by the view, puts light in model view space
		auto actor = owner->scene->FindActor("camera");
		if (actor != nullptr)
		{
			position = actor->GetComponent<CameraComponent>()->view * glm::vec4{ owner->transform.position, 1 };
		}

		// get all shaders in the resource system
		auto shaders = owner->scene->engine->Get<nc::ResourceSystem>()->Get<nc::Program>(); //< using the new Resource System Get() function, get all shaders(Program))
		// set shaders light properties
			for (auto& shader : shaders)
			{
				shader->SetUniform("light.ambient", ambient);
				//<set light diffuse, specular, and position uniforms>
				shader->SetUniform("light.diffuse", diffuse); 
				shader->SetUniform("light.specular", specular); 
				shader->SetUniform("light.position", position); 
			}

	}

	bool LightComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool LightComponent::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, ambient);
		JSON_READ(value, diffuse);
		JSON_READ(value, specular);

		return false;
	}
}