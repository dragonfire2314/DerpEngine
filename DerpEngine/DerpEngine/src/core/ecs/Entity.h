#pragma once

#include "Component.h"

#include <unordered_map>

namespace DERP {

	class DERP_API Entity
	{
	friend class ComponentManager;
	friend class EntityManager;

	private:
		//Unique ID for the entity
		uint32_t ID;
		//Entity Name
		std::string name = "New Entity";

		//Components as a key, index to the location in the component array as value
		std::unordered_map<Component*, bool> componentList;

		//Entity Parent
		Entity* parent;
		//Entity Children
		std::vector<Entity*> children;
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

		Entity* getParent();
		std::vector<Entity*>& getChildren();

		std::string getName();
		void setName(std::string s);

		std::unordered_map<Component*, bool>& OLD_getComponentMap();
	};

}

