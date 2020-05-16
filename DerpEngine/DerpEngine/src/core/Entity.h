#pragma once

#include "Component.h"
#include "Components.h"

#include <unordered_map>

namespace DERP {

	class DERP_API Entity
	{
	friend class ComponentManager;

	private:
		//Unique ID for the entity
		uint32_t ID;

		//Components as a key, index to the location in the component array as value
		//std::unordered_map<Components, Component> componentList;

		//Add the comment specified to the entitys component list
		
	public:
		Entity();
		~Entity();

		inline size_t getID() { return ID; }

		void addComponent(Component comp, uint32_t index);
	};

}

