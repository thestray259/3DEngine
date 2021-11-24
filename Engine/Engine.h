#pragma once

#define REGISTER_CLASS(class) nc::ObjectFactory::Instance().Register<class>(#class); 

// core 
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"
#include "Core/Json.h"
#include "Core/Serializable.h" 

// framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/Factory.h" 

// math
#include "Math/MathTypes.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"
#include "Math/Random.h"

// audio 
#include "Audio/AudioSystem.h" 
#include "Audio/AudioChannel.h" 

// graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/Program.h"
#include "Graphics/Material.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexIndexBuffer.h"

// input 
#include "Input/InputSystem.h"

// resource
#include "Resource/ResourceSystem.h"

// objects 
#include "Object/Actor.h"
#include "Object/Scene.h"

// components 
#include "Component/PhysicsComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/AudioComponent.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace nc
{
	using ObjectFactory = Singleton<Factory<std::string, Object>>; 

	class Engine
	{
	public:
		void Startup(); 
		void Shutdown(); 

		void Update();
		void Draw(Renderer* renderer); 

		template<typename T>
		T* Get(); 

	public: 
		FrameTimer time; 

	private:
		std::vector<std::unique_ptr<System>> systems; 
	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get()); 
		}

		return nullptr;
	}
}