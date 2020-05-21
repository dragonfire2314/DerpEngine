#pragma once
#include <DerpEngine.h>

using namespace DERP;

extern MeshManager meshManager;
extern ApplicationEditor app;

struct ModelPassData {
	int modelID;
	int meshID;
};

struct ShaderPassData {
	std::string fileName;
	std::string filePath;
};

struct TexturePassData {
	int TexID;
	std::string fileName;
};

struct MaterialPassData {
	objl::Material* mat;
	std::string fileName;
};

void AssetWindow();

void ImportModel(std::string path, std::string fName);
void ImportShader(std::string path, std::string fName);
void ImportImage(std::string path, std::string fName);