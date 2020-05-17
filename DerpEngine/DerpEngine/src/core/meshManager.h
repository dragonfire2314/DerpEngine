#pragma once

#include "core.h"
#include "ecs/components/ComponentStructs.h"

#include "../../external/objLoader/OBJ_Loader.h"
#include <unordered_map>

namespace DERP {

	class DERP_API meshManager
	{
	private:
		objl::Loader* loader;

		uint32_t counter = 0;

		//Mesh map
		//Key - Mesh ID
		//Value - Mesh
		std::unordered_map<uint32_t, objl::Mesh> meshes;
	public:
		meshManager();
		~meshManager();

		bool loadMeshes(std::string filePath);
	};

}