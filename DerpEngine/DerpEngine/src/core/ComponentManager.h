#pragma once

#include "core.h"

#include "Components.h"
#include "Entity.h"

namespace DERP {

	class ComponentManager
	{
	private:

	public:
		//Basic singleton method
		DERP_API static ComponentManager& getInstance()
		{
			static ComponentManager instance;
			return instance;
		}

		ComponentManager();
		~ComponentManager();

		//C++ 11 something?
		ComponentManager(ComponentManager const&) = delete;
		void operator=(ComponentManager const&)   = delete;

		DERP_API void addComponent(Components comp, Entity e);
	};

}

