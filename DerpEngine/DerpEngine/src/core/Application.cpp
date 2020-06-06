#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../renderer/OpenGL/OpenGL.h"

#include "ecs/Components.h"
#include "ecs/systems/Renderer.h"
#include "ecs/SystemManager.h"
#include "ecs/systems/System.h"
#include "audio/AudioManager.h"
#include "Animation/AnimationTest.h"

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
			Script* s = CM::GetComponent<Script>(x);
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
			TIME::deltaTime = currentTime - lastFrameTime;
			TIME::time = currentTime;
			lastFrameTime = currentTime;

			//Update Audio
			AUDIO::updateAudio();

			//Update Animation
			//updateAnimations();

			//Update Physics
			PHYS::updatePhysics();

			//Clear screen
			renderer->ClearScreen(); 
			//Run physics

			//Render
			renderer->Render();

			//Run scripts Update
			for (auto x : sys_scripts->Entities) {
				CM::GetComponent<Script>(x)->script->Update();
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
			CM::RegisterComponent<Transform>();
			CM::RegisterComponent<RigidBody>();
			CM::RegisterComponent<Mesh>();
			CM::RegisterComponent<Material>();
			CM::RegisterComponent<Camera>();
			CM::RegisterComponent<Script>();
			CM::RegisterComponent<DirectionalLight>();
			CM::RegisterComponent<PointLight>();
			CM::RegisterComponent<BoxCollider>();
			CM::RegisterComponent<SphereCollider>();
			CM::RegisterComponent<AudioSource>();
			CM::RegisterComponent<AudioListener>();
			CM::RegisterComponent<Animator>();
			//ECS System init
			sys_renderer = systemManager.RegisterSystem<Sys_Renderer>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<Material>());
				sig.set(CM::GetComponentID<Mesh>());
				systemManager.SetSignature<Sys_Renderer>(sig);
			}

			sys_shader = systemManager.RegisterSystem<Sys_Shader>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<Material>());
				systemManager.SetSignature<Sys_Shader>(sig);
			}

			sys_vertex = systemManager.RegisterSystem<Sys_Vertex>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<Material>());
				systemManager.SetSignature<Sys_Vertex>(sig);
			}

			sys_scripts = systemManager.RegisterSystem<Sys_Scripts>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<Script>());
				systemManager.SetSignature<Sys_Scripts>(sig);
			}

			sys_cameras = systemManager.RegisterSystem<Sys_Cameras>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<Camera>());
				systemManager.SetSignature<Sys_Cameras>(sig);
			}

			sys_physics = systemManager.RegisterSystem<Sys_Physics>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<RigidBody>());
				systemManager.SetSignature<Sys_Physics>(sig);
			}

			sys_dirLight = systemManager.RegisterSystem<Sys_DirLight>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<DirectionalLight>());
				systemManager.SetSignature<Sys_DirLight>(sig);
			}

			sys_pointLight = systemManager.RegisterSystem<Sys_PointLight>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<PointLight>());
				systemManager.SetSignature<Sys_PointLight>(sig);
			}

			sys_boxCollider = systemManager.RegisterSystem<Sys_BoxCollider>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<BoxCollider>());
				systemManager.SetSignature<Sys_BoxCollider>(sig);
			}

			sys_sphereCollider = systemManager.RegisterSystem<Sys_SphereCollider>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<SphereCollider>());
				systemManager.SetSignature<Sys_SphereCollider>(sig);
			}

			sys_audio = systemManager.RegisterSystem<Sys_Audio>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<AudioSource>());
				systemManager.SetSignature<Sys_Audio>(sig);
			}

			sys_animator = systemManager.RegisterSystem<Sys_Animator>();
			{
				std::bitset<UINT8_MAX> sig;
				sig.set(CM::GetComponentID<Animator>());
				systemManager.SetSignature<Sys_Animator>(sig);
			}
		}
		/*
			INPUT INIT
		*/
		{
			input.Init((GLFWwindow*)window);
		}
		/*
			AUDIO INIT
		*/
		AUDIO::initAudio();
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