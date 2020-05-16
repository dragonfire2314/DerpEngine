#pragma once
#include "core.h"

#include "Entity.h"

#include <unordered_map>

namespace DERP {

	class EntityManager
	{
	private:
		uint32_t counter = 0;

		std::unordered_map<uint32_t, Entity*> entityMap;
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

		DERP_API Entity* createEntity();
	};

}