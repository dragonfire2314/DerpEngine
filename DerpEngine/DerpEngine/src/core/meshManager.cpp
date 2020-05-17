#include "MeshManager.h"

namespace DERP {

	MeshManager::MeshManager() 
	{

	}

	MeshManager::~MeshManager() 
	{

	}

	int MeshManager::loadMeshes(std::string filePath)
	{
		//Generate new Loader
		objl::Loader* loader = new objl::Loader();

		bool isLoaded = loader->LoadFile(filePath);

		if (!isLoaded)
			return -1;

		meshes.insert({ counter, loader });
		counter++;

		//Delete Loader
		//delete loader;

		return counter - 1;
	}

	objl::Mesh* MeshManager::getMesh(uint32_t fileID, uint32_t meshID)
	{
		return nullptr;
	}

	//Debug
	void MeshManager::debug_print_mesh(uint32_t key) 
	{
		for (int i = 0; i < meshes[key]->LoadedMeshes.size(); i++) 
		{
			objl::Mesh curMesh = meshes[key]->LoadedMeshes[i];

			printf("%i :Mesh name: %s", i, curMesh.MeshName.c_str());
		}
	}
}