#include <DerpEngine.h>
#include <stdio.h>

#include <external/imgui/imgui.h>
#include <external/imgui/ImGuiFileBrowser.h>

#include "Scene.h"
#include "ComponentsVeiw.h"
#include "AssetWidow.h"

#include "CameraMovement.h"
#include "../testScripts/RotateAroundSceen.h"

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

    //Import some shaders
    ImportShader("pixel.glsl", "pixel.glsl");
    ImportShader("vertex.glsl", "vertex.glsl");
    //Import some models
    //ImportModel("../models/Treasure_Chest/treasure_chest2.obj", "treasure_chest2.obj");
    //ImportModel("../models/handmade/cube.obj", "cube.obj");
    //ImportModel("../models/handmade/cubeSmall.obj", "cubeSmall.obj");
    int temp = meshManager.loadMeshes("../models/handmade/cube.obj");

    if (temp == -1)
    {
        printf("Bad Dir\n");
    }
    int temp2 = meshManager.loadMeshes("../models/Treasure_Chest/treasure_chest2.obj");

    if (temp2 == -1)
    {
        printf("Bad Dir\n");
    }

    printf("this: %i, %i\n", temp, temp2);

    //Sample
    uint32_t ent = EntityManager::CreateEntity();
    EntityManager::getEntity(ent)->name = "Main Object";
    cm.AddComponent<Transform>(ent);
    cm.AddComponent<Mesh>(ent);
    cm.AddComponent<Material>(ent);
    cm.GetComponent<Mesh>(ent)->mesh = meshManager.getMesh(temp2, 0);

    //Create a camera
    uint32_t cam = EntityManager::CreateEntity();
    EntityManager::getEntity(cam)->name = "Camera";
    ComponentManager::AddComponent<Camera>(cam);
    ComponentManager::AddComponent<Transform>(cam);
    Transform* cam_t = ComponentManager::GetComponent<Transform>(cam);
    cam_t->position.z = 4;
    cm.AddComponent<Script>(cam);
    Script* s3 = ComponentManager::GetComponent<Script>(cam);
    s3->script = new CameraMovement();

    //Create a Point Light
    uint32_t pLight = EntityManager::CreateEntity();
    EntityManager::getEntity(pLight)->name = "Light";
    cm.AddComponent<PointLight>(pLight);
    cm.AddComponent<Transform>(pLight);
    //cm.AddComponent<Script>(pLight);
    //cm.GetComponent<Script>(pLight)->script = new RotateAroundScene();
    cm.AddComponent<Mesh>(pLight);
    cm.AddComponent<Material>(pLight);
    cm.GetComponent<Mesh>(pLight)->mesh = meshManager.getMesh(temp, 0);
    cm.GetComponent<Transform>(pLight)->position.x = 2;
    

	app.setUpdate(Update);

    app.startUp();
	
    app.Run();

	return 0;
}