#include "Scene.h"

#include <external/imgui/imgui.h>

//Functions
void addEntity(std::string name);
void removeEntity();

//Stuff for scene window
std::vector<uint32_t> v;

ImGuiTreeNodeFlags base_flags =
ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow |
ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
bool test_drag_and_drop = false;

//Defualt selection
int selectionID = -1;
int node_clicked = -1;

void traverseTree(Entity* node)
{
    ImGuiTreeNodeFlags node_flags = base_flags;
    const bool is_selected = ((node->ID) == selectionID);
    if (is_selected)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    std::vector<uint32_t> childs = node->childern;

    if (childs.empty()) //Has no childern 
    {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)node->ID, node_flags, "name");
        if (ImGui::IsItemClicked()) {
            node_clicked = node->ID;
            selectionID = node->ID;
        }
    }
    else //Has children
    {
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)(node->ID), node_flags, "name");
        if (ImGui::IsItemClicked()) {
            node_clicked = (node->ID);
            selectionID = node->ID;
        }

        if (node_open) {

            if (childs.empty()) {
                ImGui::TreePop();
                return;
            }

            for (auto y : childs) {
                traverseTree(EntityManager::getEntity(y));
            }

            ImGui::TreePop();
        }
    }
}

void sceneWindow() 
{
    v = EntityManager::getEntity(-2)->childern;

    //Scene Window
    ImGui::Begin("Scene");

    static char str0[128] = "New Entity";

    //Buttons
    //Other green Add
    ImGui::PushID(2);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
    if (ImGui::Button("Add Entity To Root")) {
        ImGui::OpenPopup("EntityName2");
        //addEntity();
    }
    if (ImGui::BeginPopup("EntityName2"))
    {
        ImGui::Text("Enter Entity Name");

        ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
        if (ImGui::Button("Enter")) {
            //str0 = "New Entity";
            addEntity(str0);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();
    //Green Add
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
    if (ImGui::Button("Add Entity")) {
        ImGui::OpenPopup("EntityName");
        //addEntity();
    }
    if (ImGui::BeginPopup("EntityName"))
    {
        ImGui::Text("Enter Entity Name");
        
        ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
        if (ImGui::Button("Enter")) {
            //str0 = "New Entity";
            addEntity(str0);
            ImGui::CloseCurrentPopup();
        }
            
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    //Red Remove
    ImGui::PushID(1);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
    if (ImGui::Button("Remove Entity")) {
        removeEntity();
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::Separator();

    //Dig through node tree
    for (auto x : v)
    {
        Entity* e = EntityManager::getEntity(x);

        ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = ((e->ID) == selectionID);
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;

        std::vector<uint32_t> childs = e->childern;

        if (childs.empty()) //Has no childern 
        {
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx((void*)(intptr_t)e->ID, node_flags, "Name");
            if (ImGui::IsItemClicked()) {
                node_clicked = e->ID;
                selectionID = e->ID;
            }
        }
        else //Has children
        {
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)(e->ID), node_flags, "Name");
            if (ImGui::IsItemClicked()) {
                node_clicked = (e->ID);
                selectionID = e->ID;
            }

            if (node_open) {

                if (childs.empty()) {
                    ImGui::TreePop();
                    return;
                }

                for (auto y : childs) {
                    traverseTree(EntityManager::getEntity(y));
                }

                ImGui::TreePop();
            }
        }
    }
    if (node_clicked != -1)
    {
        // Update selection state
        // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
       // if (ImGui::GetIO().KeyCtrl)
            //selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
        //else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
        //selectionID = -1;


        printf("Node Cicked %i\n", node_clicked);

        node_clicked = -1;
    }

    ImGui::End();
}

void addEntity(std::string name) 
{
    if (selectionID == -1) {
        //EntityManager::getInstance().createEntity()->setName(name);
        EntityManager::CreateEntity();
    }
    else {
        //EntityManager::getInstance().createEntity(EntityManager::getInstance().getEntity(selectionID))->setName(name);
        EntityManager::CreateEntity(selectionID);
    }
}

void removeEntity() 
{
    //Remove Entity that is selected
    //EntityManager::getInstance().removeEntity(EntityManager::getInstance().getEntity(selectionID));
}