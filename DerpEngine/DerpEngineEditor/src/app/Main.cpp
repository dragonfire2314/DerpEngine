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

void Update(void* screenImage)
{
    //Menu Bar
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
    }
    //Dockspace
    {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
    }
    //Windows
    AssetWindow();

    sceneWindow();

    ComponentsWindow();
    //View
    {
        ImGui::Begin("View");

        ImVec2 vMax = ImGui::GetContentRegionAvail();

        printf("VMAX.x, VMAX.y: %f, %f\n", vMax.x, vMax.y);
        printf("V2AX.x, V2AX.y: %i, %i\n", (uint32_t)vMax.x, (uint32_t)vMax.y);

        app.setWidth((uint32_t)vMax.x);
        app.setHeight((uint32_t)vMax.y);

        //Should only call when resolution changes
        app.UpdateResolution();

        ImVec2 uv_min = ImVec2(0.0f, 1.0f);
        ImVec2 uv_max = ImVec2(1.0f, 0.0f);
        uint32_t tttt = *(uint32_t*)screenImage;
        ImGui::Image((void*)tttt, vMax, uv_min, uv_max);

        ImGui::End();
    }

    ImGui::End();
}

int main() 
{
    app.Init(1024, 768);
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

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
    app.Run();

	return 0;
}