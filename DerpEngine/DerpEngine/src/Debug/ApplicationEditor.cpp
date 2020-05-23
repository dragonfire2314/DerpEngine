#include "ApplicationEditor.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "../../external/imgui/imgui_impl_opengl3.h"

namespace DERP {

	ApplicationEditor::ApplicationEditor()
	{

	}

	ApplicationEditor::~ApplicationEditor()
	{

	}

	void ApplicationEditor::startUp()
	{
		start();

		//Loader temp shader
		renderer->SetUp();

		Start3();
	}

	void ApplicationEditor::Run()
	{
		//Engine loop
		do {
			//Clear screen
			renderer->ClearScreen();
			//Run physics

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Update3();
			updateFunc();

			

			//Run user callback

			//Render
			renderer->Render();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Swap buffers
			glfwSwapBuffers((GLFWwindow*)window);
			glfwPollEvents();

		} while (glfwGetKey((GLFWwindow*)window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose((GLFWwindow*)window) == 0);

		//Shutdown ImGui?
		//Shutdown GLFW?
	}

	void ApplicationEditor::Update3()
	{
		
	}

	void ApplicationEditor::setUpdate(void(*func)())
	{
		updateFunc = func;
	}

	RenderAPI* ApplicationEditor::getRenderAPI()
	{
		return renderer;
	}

	void ApplicationEditor::Start3()
	{
		const char* glsl_version = "#version 130";

		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

}