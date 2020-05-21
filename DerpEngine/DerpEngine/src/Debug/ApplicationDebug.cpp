#include "ApplicationDebug.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../core/ecs/components/Transform.h"

#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "../../external/imgui/imgui_impl_opengl3.h"


namespace DERP {

	ApplicationDebug::ApplicationDebug()
	{

	}

	ApplicationDebug::~ApplicationDebug()
	{

	}

	int counter = 0;

	void ApplicationDebug::Run() 
	{
		//Engine setup
		getCS()->startComponent();

		start();
		debugStart();

		//Loader temp shader
		renderer->SetUp();
		//glUseProgram(renderer->LoadShader("vertex.v", "pixel.p"));

		//Engine loop
		do {
			//Clear screen
			renderer->ClearScreen();
			//Run physics

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			float f;
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			//Run scripts
			getCS()->updateComponent();

			//Render
			renderer->Render();

			// Swap buffers
			glfwSwapBuffers((GLFWwindow*)window);
			glfwPollEvents();

		} while (glfwGetKey((GLFWwindow*)window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose((GLFWwindow*)window) == 0);

		//Shutdown ImGui?
		//Shutdown GLFW?
	}

	void ApplicationDebug::debugStart()
	{
		const char* glsl_version = "#version 130";

		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

}