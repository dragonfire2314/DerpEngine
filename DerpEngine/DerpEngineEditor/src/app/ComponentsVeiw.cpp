#include "ComponentsVeiw.h"

#include <external/imgui/imgui.h>

#include "AssetWidow.h"
#include "Scene.h"

void TransformComponent();
void MeshComponent();
void MaterialComponent();

ComponentManager componentManager;

void ComponentsWindow() 
{
    if (!(EntityManager::getInstance().getEntity(selectionID) == nullptr))
    {
        ImGui::Begin("Components");

        //Entity
        ImGui::Text("%s", EntityManager::getInstance().getEntity(selectionID)->getName().c_str());
        ImGui::Separator();

        //Green Add
        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("Component");
        }
        if (ImGui::BeginPopup("Component"))
        {
            ImGui::Text("Select Component");
            ImGui::Separator();
            if (ImGui::Selectable("Transform")) {
                if (!EntityManager::getInstance().getEntity(selectionID)->getComponent<Transform>(ComponentTransform::getInstance()))
                    componentManager.addComponent(ComponentTransform::getInstance(), EntityManager::getInstance().getEntity(selectionID));
            }
            if (ImGui::Selectable("Mesh")) {
                if (!EntityManager::getInstance().getEntity(selectionID)->getComponent<Mesh>(ComponentMesh::getInstance())) {
                    componentManager.addComponent(ComponentMesh::getInstance(), EntityManager::getInstance().getEntity(selectionID));
                }
            }
            if (ImGui::Selectable("Material")) {
                if (!EntityManager::getInstance().getEntity(selectionID)->getComponent<Material>(ComponentMaterial::getInstance())) {
                    componentManager.addComponent(ComponentMaterial::getInstance(), EntityManager::getInstance().getEntity(selectionID));
                }
            }
            
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::Separator();
        

        std::unordered_map<Component*, bool> data = EntityManager::getInstance().getEntity(selectionID)->OLD_getComponentMap();

        for (auto x : data) 
        {
            if (x.second) 
            {
                if (ComponentTransform::getInstance() == x.first) {
                    TransformComponent();
                }
                if (ComponentMesh::getInstance() == x.first) {
                    MeshComponent();
                }
                if (ComponentMaterial::getInstance() == x.first) {
                    MaterialComponent();
                }
            }
        }

        ImGui::End();
    }
}

void TransformComponent() 
{
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None)) 
    {
        const float fm = -1000000.0;
        const float fM = 1000000.0;
        Transform* t = EntityManager::getInstance().getEntity(selectionID)->getComponent<Transform>(ComponentTransform::getInstance());
        //Position
        ImGui::Text("Position");
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("PX", ImGuiDataType_Float, &t->position.x, 0.005f, &fm, &fM, "%f", 1.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("PY", ImGuiDataType_Float, &t->position.y, 0.005f, &fm, &fM, "%f", 1.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("PZ", ImGuiDataType_Float, &t->position.z, 0.005f, &fm, &fM, "%f", 1.0f);
        //Rotation
        glm::vec3 euler = glm::eulerAngles(t->rotation) * 180.0f / 3.14159f;
        ImGui::Text("Rotation");
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("RX", ImGuiDataType_Float, &euler.x, 0.200f, &fm, &fM, "%f", 1.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("RY", ImGuiDataType_Float, &euler.y, 0.200f, &fm, &fM, "%f", 1.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("RZ", ImGuiDataType_Float, &euler.z, 0.200f, &fm, &fM, "%f", 1.0f);
        euler = euler * 3.14159f / 180.0f;
        t->rotation = glm::quat(euler);
        //Scale
        ImGui::Text("Scale");
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("SX", ImGuiDataType_Float, &t->scale.x, 0.005f, &fm, &fM, "%f", 1.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("SY", ImGuiDataType_Float, &t->scale.y, 0.005f, &fm, &fM, "%f", 1.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.23f);
        ImGui::DragScalar("SZ", ImGuiDataType_Float, &t->scale.z, 0.005f, &fm, &fM, "%f", 1.0f);

        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
        if (ImGui::SmallButton("Remove Component")) {

        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
}

std::unordered_map<uint32_t, uint32_t> entityToMeshID;

void MeshComponent()
{
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_None)) {

        Mesh* m = EntityManager::getInstance().getEntity(selectionID)->getComponent<Mesh>(ComponentMesh::getInstance());

        if (m->mesh != nullptr) {
            ImGui::Text("Mesh: %s", m->mesh->MeshName.c_str());
        }
        else 
        {
            if (entityToMeshID.find(selectionID) == entityToMeshID.end()) {
                ImGui::Text("Mesh: %i", -1);
            }
            else {
                ImGui::Text("Mesh: %i", entityToMeshID[selectionID]);
            }
            
        }

        ImGui::PushID(0);
        ImGui::Button("Mesh", ImVec2(60, 60));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MESH_PAYLOAD"))
            {
                IM_ASSERT(payload->DataSize == sizeof(ModelPassData));
                ModelPassData payload_n = *(const ModelPassData*)payload->Data;

                if (entityToMeshID.find(selectionID) == entityToMeshID.end()) {
                    entityToMeshID.insert({ selectionID , payload_n.modelID });
                }
                else {
                    entityToMeshID[selectionID] = payload_n.modelID;
                }
                
                m->mesh = meshManager.getMesh(payload_n.modelID, payload_n.meshID);
                //Material* mat = EntityManager::getInstance().getEntity(selectionID)->getComponent<Material>(ComponentMaterial::getInstance());
                //mat->mat = meshManager.getMaterial(payload_n, 0);
                app.getRenderAPI()->updateMesh(selectionID);
                app.getRenderAPI()->updateShader(selectionID);
            }
        }
        ImGui::PopID();

        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
        if (ImGui::SmallButton("Remove Component")) {

        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
}

void MaterialComponent() 
{
    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_None)) {

        Material* m = EntityManager::getInstance().getEntity(selectionID)->getComponent<Material>(ComponentMaterial::getInstance());

        //Vertex Shader
        ImGui::PushID(1);
        //ImGui::Combo("Pixel Shader", &item_current, items, IM_ARRAYSIZE(items));
        static std::string this_vertexSahder_name = "None";
        ImGui::Button(this_vertexSahder_name.c_str(), ImVec2(180, 20));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SHADER_PAYLOAD"))
            {
                IM_ASSERT(payload->DataSize == sizeof(ShaderPassData));
                ShaderPassData payload_n = *(const ShaderPassData*)payload->Data;
                printf("Shader: %s\n", payload_n.filePath.c_str());
                m->vertexShader = payload_n.filePath;
                this_vertexSahder_name = payload_n.fileName;
                app.getRenderAPI()->updateShader(selectionID);
            }
        }
        ImGui::SameLine();
        ImGui::Text("Vertex Shader");
        ImGui::PopID();
        //Pixel Sahder
        ImGui::PushID(1);
        //ImGui::Combo("Pixel Shader", &item_current, items, IM_ARRAYSIZE(items));
        static std::string this_pixelSahder_name = "None";
        ImGui::Button(this_pixelSahder_name.c_str(), ImVec2(180, 20));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SHADER_PAYLOAD"))
            {
                IM_ASSERT(payload->DataSize == sizeof(ShaderPassData));
                ShaderPassData payload_n = *(const ShaderPassData*)payload->Data;
                printf("Shader: %s\n", payload_n.filePath.c_str());
                m->pixelShader = payload_n.filePath;
                this_pixelSahder_name = payload_n.fileName;
                app.getRenderAPI()->updateShader(selectionID);
            }
        }
        ImGui::SameLine();
        ImGui::Text("Pixel Shader");
        ImGui::PopID();

        //objl::Material
        ImGui::PushID(0);
        ImGui::Text("Material");
        static std::string this_diffuseTexture_name = "None";
        ImGui::Button(this_diffuseTexture_name.c_str(), ImVec2(60, 60));
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MATERIAL_PAYLOAD"))
            {
                IM_ASSERT(payload->DataSize == sizeof(MaterialPassData));
                MaterialPassData payload_n = *(const MaterialPassData*)payload->Data;
                printf("Shader: %s\n", payload_n.fileName.c_str());
                this_diffuseTexture_name = payload_n.fileName;
                m->mat = payload_n.mat;
                app.getRenderAPI()->updateTexture(selectionID);
            }
        }
        ImGui::PopID();

        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
        if (ImGui::SmallButton("Remove Component")) {

        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
}