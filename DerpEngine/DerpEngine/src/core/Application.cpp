#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../renderer/OpenGL/OpenGL.h"

#include "ecs/Components.h"
#include "ecs/systems/Renderer.h"
#include "ecs/SystemManager.h"
#include "ecs/systems/System.h"

namespace DERP {

	Application::Application()
	{
		//New ECS startup
		printf("App Started\n");

		//Choose Renderer to use
		//Set to openGL for now
		renderer = new OpenGL();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		//Run scripts Start
		for (auto x : sys_scripts->Entities) {
			//Assign entityID's to the scripts
			Script* s = ComponentManager::GetComponent<Script>(x);
			s->script->entity = x;
			s->script->Start();
		}

		//Loader temp shader
		renderer->SetUp();

		float lastFrameTime = 0.0f;

		//Engine loop
		do {
			//Time keeping
			float currentTime = glfwGetTime();
			time.deltaTime = currentTime - lastFrameTime;
			lastFrameTime = currentTime;

			//Clear screen
			renderer->ClearScreen(); 
			//Run physics

			//Render
			renderer->Render();

			//Run scripts Update
			for (auto x : sys_scripts->Entities) {
				ComponentManager::GetComponent<Script>(x)->script->Update();
			}

			// Swap buffers
			glfwSwapBuffers((GLFWwindow*)window);
			glfwPollEvents();

		} while (glfwGetKey((GLFWwindow*)window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose((GLFWwindow*)window) == 0);
	}

	void Application::Init()
	{
		/*

			GLFW INIT

		*/
		start();
		/*
		
			ECS INIT
		
		*/
		{
			//ECS component init
			componentManager.RegisterComponent<Transform>();
			componentManager.RegisterComponent<RigidBody>();
			componentManager.RegisterComponent<Mesh>();
			componentManager.RegisterComponent<Material>();
			componentManager.RegisterComponent<Camera>();
			componentManager.RegisterComponent<Script>();
			componentManager.RegisterComponent<DirectionalLight>();
			componentManager.RegisterComponent<PointLight>();
			//ECS System init
			sys_renderer = systemManager.RegisterSystem<Sys_Renderer>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(componentManager.GetComponentID<Material>());
				sig.set(componentManager.GetComponentID<Mesh>());
				systemManager.SetSignature<Sys_Renderer>(sig);
			}

			sys_shader = systemManager.RegisterSystem<Sys_Shader>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(componentManager.GetComponentID<Material>());
				systemManager.SetSignature<Sys_Shader>(sig);
			}

			sys_vertex = systemManager.RegisterSystem<Sys_Vertex>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(componentManager.GetComponentID<Material>());
				systemManager.SetSignature<Sys_Vertex>(sig);
			}

			sys_scripts = systemManager.RegisterSystem<Sys_Scripts>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(componentManager.GetComponentID<Script>());
				systemManager.SetSignature<Sys_Scripts>(sig);
			}

			sys_cameras = systemManager.RegisterSystem<Sys_Cameras>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(componentManager.GetComponentID<Camera>());
				systemManager.SetSignature<Sys_Cameras>(sig);
			}

			sys_dirLight = systemManager.RegisterSystem<Sys_DirLight>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(componentManager.GetComponentID<DirectionalLight>());
				systemManager.SetSignature<Sys_DirLight>(sig);
			}

			sys_pointLight = systemManager.RegisterSystem<Sys_PointLight>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(componentManager.GetComponentID<PointLight>());
				systemManager.SetSignature<Sys_PointLight>(sig);
			}
		}
		/*
			INPUT INIT
		*/
		{
			input.Init((GLFWwindow*)window);
		}
	}

	void Application::start()
	{
		// Initialise GLFW
		glewExperimental = true; // Needed for core profile
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
		}

		glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

		// Open a window and create its OpenGL context
		// (In the accompanying source code, this variable is global for simplicity)
		window = glfwCreateWindow(1024, 768, "Derp Engine", NULL, NULL);
		if (window == NULL) {
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
			glfwTerminate();
		}
		glfwMakeContextCurrent((GLFWwindow*)window); // Initialize GLEW
		glewExperimental = true; // Needed in core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
		}

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode((GLFWwindow*)window, GLFW_STICKY_KEYS, GL_TRUE);
	}
}