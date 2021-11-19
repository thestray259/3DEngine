#include "Engine.h" 
#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

// vertices
const float vertices[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 
	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f 
};

const GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

// vertex shader
const char* vertexSource = R"(
    #version 430 core 
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 color;

	out vec3 fs_color; 

	uniform float scale; 

    void main()
    {
		fs_color = color; 
        gl_Position = vec4(position * scale, 1.0);
    }
)";

// fragment
const char* fragmentSource = R"(
    #version 430 core
	in vec3 fs_color; 

    out vec4 outColor;
	
	uniform vec3 tint; 

    void main()
    {
        outColor = vec4(fs_color, 1.0) * vec4(tint, 1.0);
    }
)";

int main(int argc, char** argv)
{
	nc::Engine engine;
	engine.Startup();
	engine.Get<nc::Renderer>()->Create("OpenGL", 800, 600);

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");

	std::shared_ptr<nc::Program> program = engine.Get<nc::ResourceSystem>()->Get<nc::Program>("basic_program"); 
	std::shared_ptr<nc::Shader> vshader = engine.Get<nc::ResourceSystem>()->Get<nc::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<nc::Shader> fshader = engine.Get<nc::ResourceSystem>()->Get<nc::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);
	std::shared_ptr<nc::VertexIndexBuffer> vertexBuffer = engine.Get<nc::ResourceSystem>()->Get<nc::VertexIndexBuffer>("vertex_index_buffer");

	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 4, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 6, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(float), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(float), (3 * sizeof(float)));

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

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

	// uv 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// texture 
	nc::Texture texture; 
	texture.CreateTexture("textures/llama.jpg");
	texture.Bind(); 

	// uniform 
	float time = 1; 
	program->SetUniform("scale", time);

	glm::vec3 tint{ 1.0f, 0.5f, 0.5f }; 
	program->SetUniform("tint", tint);

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
		engine.Update(); 

		time += engine.time.deltaTime; 
		program->SetUniform("scale", std::sin(time)); 

		engine.Get<nc::Renderer>()->BeginFrame(); 

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
		vertexBuffer->Draw();

		engine.Get<nc::Renderer>()->EndFrame(); 
	}

	return 0;
}
