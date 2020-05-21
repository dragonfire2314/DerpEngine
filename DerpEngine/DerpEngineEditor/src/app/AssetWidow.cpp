#include "AssetWidow.h"

#include <external/imgui/imgui.h>

#include "Scene.h"

MeshManager meshManager;
ApplicationEditor app;

std::vector<int> assetIDs;
//Key - assetID
//Value - file Name
std::unordered_map<int, std::string> assetNameMap;
//Key - Shader File Name
//Value - file Path
std::unordered_map<std::string, std::string> ShaderNameToPathMap;
//Key - texture File Name
//Value - file Path
std::unordered_map<std::string, std::string> TextureNameToPathMap;
//Key - MaterialID
struct this_matHolder {
    std::string name;
    objl::Material* mat;
};
std::unordered_map<int, this_matHolder> MaterialsMap;

ModelPassData modelPassData;
ShaderPassData shaderPassData;
TexturePassData texturePassData;
MaterialPassData materialPassData;

std::string this_diffuseTexture_name = "None";
int matSelected = -1;

void AssetWindow() 
{
    ImGui::Begin("Assets");

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Models"))
        {
            for (auto x : assetIDs)
            {
                ImGui::PushID(0);
                static int selected = -1;
                if (ImGui::CollapsingHeader(assetNameMap[x].c_str(), ImGuiTreeNodeFlags_None)) {
                    std::vector<objl::Mesh*>* tempM = meshManager.getMeshes(x);
                    for (int i = 0; i < tempM->size(); i++) {
                        char buf[32];
                        sprintf(buf, "Mesh %d", i);
                        if (ImGui::Selectable(buf, selected == i))
                            selected = i;
                        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                        {
                            // Set payload to carry the index of our item (could be anything)
                            modelPassData.modelID = x;
                            modelPassData.meshID = i;
                            ImGui::SetDragDropPayload("MESH_PAYLOAD", &modelPassData, sizeof(ModelPassData));
                            ImGui::EndDragDropSource();
                        }
                    }
                }
                ImGui::PopID();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Textures"))
        {
            for (auto x : app.textureManager.texturePaths)
            {
                ImGui::PushID(0);
                static int texSelected = -1;
                if (ImGui::Selectable(x.second.name.c_str(), false))
                    texSelected = x.first;
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    texturePassData.TexID = x.first;
                    texturePassData.fileName = x.second.name;
                    ImGui::SetDragDropPayload("TEXTURE_PAYLOAD", &texturePassData, sizeof(TexturePassData));
                    ImGui::EndDragDropSource();
                }
                ImGui::PopID();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Shaders"))
        {
            int i = 0;
            for (auto x : ShaderNameToPathMap)
            {
                ImGui::PushID(0);
                static int shaderSelected = -1;
                if (ImGui::Selectable(x.first.c_str(), shaderSelected == i))
                    shaderSelected = i;
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    shaderPassData.fileName = x.first;
                    shaderPassData.filePath = x.second;
                    ImGui::SetDragDropPayload("SHADER_PAYLOAD", &shaderPassData, sizeof(ShaderPassData));
                    ImGui::EndDragDropSource();
                }
                ImGui::PopID();
                i++;
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Materials"))
        {
            int r = 0;
            for (auto x : MaterialsMap)
            {
                ImGui::PushID(0);
                if (ImGui::Selectable(x.second.name.c_str(), matSelected == x.first)) {
                    matSelected = x.first;
                    this_diffuseTexture_name = "None";
                }
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    materialPassData.fileName = x.second.name;
                    materialPassData.mat = x.second.mat;
                    ImGui::SetDragDropPayload("MATERIAL_PAYLOAD", &materialPassData, sizeof(MaterialPassData));
                    ImGui::EndDragDropSource();
                }
                ImGui::PopID();
                r++;
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();

    ImGui::Begin("Material Editor");
    {
        if (matSelected == -1) {}
        else {

            ImGui::Text(MaterialsMap[matSelected].name.c_str());
            objl::Material* m = MaterialsMap[matSelected].mat;

            //Texture
            ImGui::PushID(0);
            ImGui::Text("Diffuse Texture");
            ImGui::Button(m->map_Kd.c_str(), ImVec2(60, 60));
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_PAYLOAD"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(TexturePassData));
                    TexturePassData payload_n = *(const TexturePassData*)payload->Data;
                    printf("Shader: %s\n", payload_n.fileName.c_str());
                    m->map_Kd_ID = payload_n.TexID;
                    this_diffuseTexture_name = payload_n.fileName;
                    m->map_Kd = payload_n.fileName;
                    //if (selectionID != -1) app.getRenderAPI()->updateTexture(selectionID);
                }
            }
            ImGui::PopID();
        }
    }
    ImGui::End();
}

void ImportModel(std::string path, std::string fName)
{
    static int counter = 0;
    int temp = meshManager.loadMeshes(path);

    if (temp == -1)
    {
        printf("Bad Dir\n");
    }

    printf("%s\n", path.c_str());

    printf("%s\n", fName.c_str());

    assetIDs.push_back(temp);
    assetNameMap.insert({ temp, fName });

    std::vector<objl::Mesh*> this_meshList = *meshManager.getMeshes(temp);

    int i = 0;
    for (auto x : this_meshList)
    {
        this_matHolder matHolder;
        matHolder.mat = &x->MeshMaterial;
        matHolder.name = fName + " " + std::to_string(i);
        MaterialsMap.insert({ counter, matHolder });
        i++;
        counter++;
    }
}

void ImportShader(std::string path, std::string fName) 
{
    ShaderNameToPathMap.insert({ fName, path });
}

void ImportImage(std::string path, std::string fName) 
{
    TextureNameToPathMap.insert({ fName, path });
    app.textureManager.addTexture(path);
}