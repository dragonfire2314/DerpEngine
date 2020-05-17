#include "meshManager.h"

namespace DERP {

	bool meshManager::loadMeshes(std::string filePath)
	{
		//Generate new Loader
		loader = new objl::Loader();

		bool isLoaded = loader->LoadFile(filePath);

		if (!isLoaded)
			return false;

		for (int i = 0; i < loader->LoadedMeshes.size(); i++) 
		{
			meshes.insert({ counter, loader->LoadedMeshes[i] });
			counter++;
		}

		//Delete Loader
		delete loader;
	}

}