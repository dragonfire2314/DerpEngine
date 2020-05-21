#pragma once
#include "../core.h"

#include "Entity.h"

#include <unordered_map>

namespace DERP {

	class EntityManager
	{
	private:
		uint32_t counter = 0;

		std::unordered_map<uint32_t, Entity*> entityMap;

		//Stores the root entity's children
		//Look thourgh children to find other entitys in the tree
		std::vector<Entity*> root;
	public:
		//Basic singleton method
		DERP_API static EntityManager& getInstance()
		{
			static EntityManager instance;
			return instance;
		}

		//C++ 11 something?
		EntityManager (EntityManager const&) = delete;
		void operator=(EntityManager const&) = delete;

		EntityManager();
		~EntityManager();

		Entity* getEntity(uint32_t ID);

		DERP_API Entity* createEntity();
		DERP_API Entity* createEntity(Entity* parent);

		DERP_API std::vector<Entity*>& getRoot();
		DERP_API void removeFromRoot(Entity* e);

		DERP_API void removeEntity(Entity* e);
	};

}