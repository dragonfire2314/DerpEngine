#include "ApplicationEditor.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "../../external/imgui/imgui_impl_opengl3.h"

#include "../core/ecs/Components.h"
#include "../core/ecs/systems/Renderer.h"
#include "../core/ecs/SystemManager.h"
#include "../core/ecs/systems/System.h"
#include "../core/audio/AudioManager.h"
#include "../core/particles/ParticleManager.h"

namespace DERP {

	ApplicationEditor::ApplicationEditor()
	{

	}

	ApplicationEditor::~ApplicationEditor()
	{

	}

	void ApplicationEditor::startUp()
	{
		//start();

		//Loader temp shader
		renderer->SetUp();

		Start3();
	}

	void ApplicationEditor::Run()
	{
		//Run scripts Start
		for (auto x : sys_scripts->Entities) {
			//Assign entityID's to the scripts
			Script* s = CM::GetComponent<Script>(x);
			s->script->entity = x;
			s->script->Start();
		}

		float lastFrameTime = 0.0f;
		//Engine loop
		do {
			//Time keeping
			float currentTime = glfwGetTime();
			TIME::deltaTime = currentTime - lastFrameTime;
			TIME::time = currentTime;
			lastFrameTime = currentTime;

			//Update ScreenRenderRes
			if (shouldUpdateRes) {
				renderer->updateFrameBuffer();
				shouldUpdateRes = false;
			}

			//Update Audio
			AUDIO::updateAudio();

			//Update Particles
			updateParitcles();

			//Update Physics
			PHYS::updatePhysics();

			//Clear screen
			//renderer->ClearScreen();
			//Run physics

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Update3();
			updateFunc(renderer->RenderToTexture());

			

			//Run user callback

			//Render
			//renderer->Render();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			//Run scripts Update
			for (auto x : sys_scripts->Entities) {
				CM::GetComponent<Script>(x)->script->Update();
			}

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

	void ApplicationEditor::setUpdate(void(*func)(void*))
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

	uint32_t ApplicationEditor::getHeight() 
	{
		return renderer->renderHeight;
	}

	uint32_t ApplicationEditor::getWidth() 
	{
		return renderer->renderWidth;
	}

	void ApplicationEditor::setHeight(uint32_t height) 
	{
		renderer->renderHeight = height;
	}

	void ApplicationEditor::setWidth(uint32_t width) 
	{
		renderer->renderWidth = width;
	}
}