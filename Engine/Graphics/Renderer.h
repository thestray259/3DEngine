#pragma once
#include "Framework/System.h"
#include "Math/Transform.h"

#include <glad/glad.h>
#include <SDL.h>
#include <string>

namespace nc
{
	class Renderer : public System
	{
	public: 
		void Startup() override;
		void Shutdown() override;
		void Update(float dt) override;

		void Create(const std::string& name, int width, int height); 
		void BeginFrame(); 
		void EndFrame(); 

	private: 
		SDL_GLContext context; 
		SDL_Window* window = nullptr; 
	};
}