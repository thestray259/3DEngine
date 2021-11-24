#include "Engine.h" 
#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

// vertices
const float vertices[] =
{
	// front
	-1.0f, -1.0f,  1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// back
	-1.0f, -1.0f, -1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -1.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
};

const GLuint indices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

int main(int argc, char** argv)
{
	// create engine 
	std::unique_ptr<nc::Engine> engine = std::make_unique<nc::Engine>(); 
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("OpenGL", 800, 600);

	// create scene
	std::unique_ptr<nc::Scene> scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");

	// create shaders 
	std::shared_ptr<nc::Program> program = engine->Get<nc::ResourceSystem>()->Get<nc::Program>("basic_shader"); 
	std::shared_ptr<nc::Shader> vshader = engine->Get<nc::ResourceSystem>()->Get<nc::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<nc::Shader> fshader = engine->Get<nc::ResourceSystem>()->Get<nc::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);
	
	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	// vertex buffer 
	std::shared_ptr<nc::VertexIndexBuffer> vertexBuffer = engine->Get<nc::ResourceSystem>()->Get<nc::VertexIndexBuffer>("cube_mesh");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 8, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(float), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(float), 3 * sizeof(float));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(float), 6 * sizeof(float));

	//// vertex array 
	//GLuint vao; 
	//glGenVertexArrays(1, &vao); 
	//glBindVertexArray(vao); 

	//// vertex buffer object
	//GLuint vbo; 
	//glGenBuffers(1, &vbo); 

	//// bind vertex buffer as active buffer (state) 
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//// set the vertex data into the vertex shader
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	//GLuint ebo; // element buffer object 
	//glGenBuffers(1, &ebo); 
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); 
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	//// position 
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0); 
	//glEnableVertexAttribArray(0); 

	//// color 
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//// uv 
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	// texture 
	{
		auto texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("textures/llama.jpg");
		texture->Bind();

		texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("textures/rocks.bmp");
		texture->Bind();

		texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("textures/wood.png");
		texture->Bind();
	}

	// create camera
	{
		auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Actor");
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 10 };

		auto component = nc::ObjectFactory::Instance().Create<nc::CameraComponent>("CameraComponent");
		component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	// create cube 
	{
		auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Actor");
		actor->name = "cube";
		actor->transform.position = glm::vec3{ 0, 0, 0 };

		auto component = nc::ObjectFactory::Instance().Create<nc::MeshComponent>("MeshComponent");
		component->program = engine->Get<nc::ResourceSystem>()->Get<nc::Program>("basic_shader");
		component->vertexBuffer = engine->Get<nc::ResourceSystem>()->Get<nc::VertexIndexBuffer>("cube_mesh");

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	// uniform 
	//float time = 1; 
	//program->SetUniform("scale", time);

	//glm::vec3 tint{ 1.0f, 0.5f, 0.5f }; 
	//program->SetUniform("tint", tint);

	//glm::mat4 view{ 1 }; 
	//view = glm::lookAt(glm::vec3{ 0, 0, 1 }, { 0, 0, 0 }, { 0, 1, 0 }); 
	//program->SetUniform("view", view); 

	glm::vec3 translate{ 0 };
	float angle = 0; 

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();
		engine->Update(); 
		scene->Update(engine->time.deltaTime);

		// update actor
		glm::vec3 direction{ 0 };
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nc::InputSystem::eKeyState::Held) direction.x = -1;
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nc::InputSystem::eKeyState::Held) direction.x = 1;
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nc::InputSystem::eKeyState::Held) direction.z = -1;
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == nc::InputSystem::eKeyState::Held) direction.z = 1;
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_E) == nc::InputSystem::eKeyState::Held) direction.y = 1;
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_Q) == nc::InputSystem::eKeyState::Held) direction.y = -1;

		auto actor = scene->FindActor("cube");
		if (actor != nullptr)
		{
			actor->transform.position += direction * 5.0f * engine->time.deltaTime;
			actor->transform.rotation.y += engine->time.deltaTime;
		}

		engine->Get<nc::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<nc::Renderer>()->EndFrame();
	}

	return 0;
}
