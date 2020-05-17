#pragma once

#include "Component.h"

#include <unordered_map>

namespace DERP {

	class DERP_API Entity
	{
	friend class ComponentManager;

	private:
		//Unique ID for the entity
		uint32_t ID;

		//Components as a key, index to the location in the component array as value
		std::unordered_map<Component*, bool> componentList;

		//Add the comment specified to the entitys component list
		
	public:
		Entity(uint32_t _ID);
		~Entity();

		inline uint32_t getID() { return ID; }

		void addComponent(Component* comp);
		bool isComponent(Component* comp);

		template <typename T>
		T* getComponent(Component* comp)
		{
			//Check if component exsists on enitiy making the call
			if (isComponent(comp))
			{
				return (T*)comp->getData(ID);
			}

			return nullptr;
		}
	};

}

