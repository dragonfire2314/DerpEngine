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
    app.Init();
    EM::Init();

    //Import some shaders
    //ImportShader("pixel.glsl", "pixel.glsl");
    //ImportShader("vertex.glsl", "vertex.glsl");
    //Import some models
    //ImportModel("../models/Treasure_Chest/treasure_chest2.obj", "treasure_chest2.obj");
    //ImportModel("../models/handmade/cube.obj", "cube.obj");
    //ImportModel("../models/handmade/cubeSmall.obj", "cubeSmall.obj");
    int cube = meshManager.loadMeshes("../models/handmade/cube.obj");
    int ico = meshManager.loadMeshes("../models/handmade/ico.obj");
    int cone = meshManager.loadMeshes("../models/handmade/cone.obj");
    int torus = meshManager.loadMeshes("../models/handmade/torus.obj");
    int plane = meshManager.loadMeshes("../models/handmade/plane.obj");
    int face = meshManager.loadMeshes("../models/handmade/face.obj");
    int chest = meshManager.loadMeshes("../models/Treasure_Chest/treasure_chest2.obj");
    int bar = meshManager.loadMeshes("../models/Chemical_Barrel/chemicalbarrel.obj");
    int testAnim = meshManager.loadMeshes("../models/anims/Swing_Dancing.fbx");

    Animation_Info info = loadAnimationFromFile("../models/anims/Swing_Dancing.fbx");

    //Chest
    uint32_t ent = EM::CreateEntity();
    EM::getEntity(ent)->name = "Chest";
    CM::AddComponent<Transform>(ent);
    CM::AddComponent<Mesh>(ent);
    CM::AddComponent<Material>(ent);
    CM::GetComponent<Mesh>(ent)->mesh = meshManager.getMesh(chest, 0);
    CM::GetComponent<Transform>(ent)->position = glm::vec3(3, -2, 0);
    CM::GetComponent<Material>(ent)->mat = meshManager.getMaterial(chest, 0);

    //Floor
    uint32_t ent3 = EM::CreateEntity();
    EM::getEntity(ent3)->name = "Plane";
    CM::AddComponent<Transform>(ent3);
    CM::AddComponent<Mesh>(ent3);
    CM::AddComponent<Material>(ent3);
    CM::AddComponent<RigidBody>(ent3);
    CM::AddComponent<BoxCollider>(ent3);
    CM::GetComponent<RigidBody>(ent3)->e = 0.4f;
    CM::GetComponent<BoxCollider>(ent3)->halfSides = glm::vec3(4, 0.01, 4);
    CM::GetComponent<Mesh>(ent3)->mesh = meshManager.getMesh(plane, 0);
    CM::GetComponent<Transform>(ent3)->position = glm::vec3(0, -2, 0);
    CM::GetComponent<Material>(ent3)->mat->Kd = objl::Vector3(0.5, 0.1, 0.7);

    //Create a camera
    uint32_t cam = EM::CreateEntity();
    EM::getEntity(cam)->name = "Camera";
    CM::AddComponent<Camera>(cam);
    CM::AddComponent<Transform>(cam);
    Transform* cam_t = CM::GetComponent<Transform>(cam);
    cam_t->position = glm::vec3(-4.44764, 3.3252, 10.8293);
    cam_t->rotation = glm::quat(glm::vec3(-0.278857, -0.310267, -0.0491606));
    CM::AddComponent<Script>(cam);
    Script* s3 = CM::GetComponent<Script>(cam);
    s3->script = new CameraMovement();

    //Create a Point Light
    uint32_t pLight = EM::CreateEntity();
    EM::getEntity(pLight)->name = "Ico";
    CM::AddComponent<DirectionalLight>(pLight);
    CM::AddComponent<Transform>(pLight);
    CM::AddComponent<Script>(pLight);
    CM::GetComponent<Script>(pLight)->script = new RotateAroundScene();
    CM::AddComponent<Mesh>(pLight);
    CM::AddComponent<Material>(pLight);
    CM::GetComponent<Mesh>(pLight)->mesh = meshManager.getMesh(ico, 0);
    CM::GetComponent<Transform>(pLight)->position.y = 1;
    
    //Anim test
    uint32_t anim = EM::CreateEntity();
    EM::getEntity(anim)->name = "anim";
    CM::AddComponent<Transform>(anim);
    CM::AddComponent<Mesh>(anim);
    CM::AddComponent<Material>(anim);
    CM::AddComponent<Animator>(anim);
    CM::GetComponent<Animator>(anim)->clipID = getAnimationClipID(info.animationGroupID, info.animationNames[0]);
    CM::GetComponent<Mesh>(anim)->mesh = meshManager.getMesh(testAnim, 0);
    CM::GetComponent<Mesh>(anim)->mesh->isAnimation = true;
    CM::GetComponent<Material>(anim)->mat->Kd = objl::Vector3(0.211, 0.509, 0.729);
    CM::GetComponent<Transform>(anim)->scale = glm::vec3(0.01, 0.01, 0.01);

	app.setUpdate(Update);

    app.startUp();
	
    app.Run();

	return 0;
}