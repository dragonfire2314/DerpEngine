#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../renderer/OpenGL/OpenGL.h"

namespace DERP {

	Application::Application()
	{
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
		//Engine setup
		getCS()->startComponent();

		start();

		//Loader temp shader
		renderer->SetUp();
		//glUseProgram(renderer->LoadShader("vertex.v", "pixel.p"));

		//Engine loop
		do {
			//Clear screen
			renderer->ClearScreen(); 
			//Run physics

			//Run scripts
			getCS()->updateComponent();

			//Render
			renderer->Render();

			// Swap buffers
			glfwSwapBuffers((GLFWwindow*)window);
			glfwPollEvents();

		} while (glfwGetKey((GLFWwindow*)window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose((GLFWwindow*)window) == 0);
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
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
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