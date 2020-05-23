#include <DerpEngine.h>
#include <stdio.h>

#include <external/imgui/imgui.h>
#include <external/imgui/ImGuiFileBrowser.h>

#include "Scene.h"
#include "ComponentsVeiw.h"
#include "AssetWidow.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


using namespace DERP;

imgui_addons::ImGuiFileBrowser file_dialog;

void Update() 
{
    //Menu Bar
    bool importModel = false, importImage = false, importShader = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::MenuItem("Import Model", NULL))
                importModel = true;
            if (ImGui::MenuItem("Import Image", NULL))
                importImage = true;
            if (ImGui::MenuItem("Import Shader", NULL))
                importShader = true;

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (importModel)
        ImGui::OpenPopup("Import Model");
    if (importImage)
        ImGui::OpenPopup("Import Image");
    if (importShader)
        ImGui::OpenPopup("Import Shader");

    if (file_dialog.showFileDialog("Import Model", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".obj"))
    {
        //Import a model
        ImportModel(file_dialog.selected_path, file_dialog.selected_fn);
    }
    if (file_dialog.showFileDialog("Import Image", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".png"))
    {
        //Import a model
        ImportImage(file_dialog.selected_path, file_dialog.selected_fn);
    }
    if (file_dialog.showFileDialog("Import Shader", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".glsl, .v, .p"))
    {
        //Import a model
        ImportShader(file_dialog.selected_path, file_dialog.selected_fn);
    }

    AssetWindow();

    sceneWindow();

    ComponentsWindow();

    ImGui::ShowDemoWindow();

}

int main() 
{
    ComponentManager cm;
    SystemManager sm;

    app.Init();
    EntityManager::Init();

	app.setUpdate(Update);

    app.startUp();
	
    app.Run();

	return 0;
}