#pragma once

#include "core.h"
#include "ecs/components/ComponentStructs.h"

#include "../../external/objLoader/OBJ_Loader.h"
#include <unordered_map>

namespace DERP {

	class DERP_API MeshManager
	{
	private:
		//Assimp Stuff

		std::unordered_map<uint32_t, std::vector<objl::Mesh*>> newMeshes;

		//End Assimp


		uint32_t counter = 0;

		//Mesh map
		//Key - Mesh ID
		//Value - Mesh
		std::unordered_map<uint32_t, objl::Loader*> meshes;
	public:
		MeshManager();
		~MeshManager();

		int loadMeshes(std::string filePath);

		objl::Mesh* getMesh(uint32_t fileID, uint32_t meshID);
		objl::Material* getMaterial(uint32_t fileID, uint32_t meshID);

		std::vector<objl::Mesh*>* getMeshes(uint32_t fileID);

		std::unordered_map<uint32_t, std::vector<objl::Mesh*>> getAllData();

		//Debug
		void debug_print_mesh(uint32_t key);
	};

}