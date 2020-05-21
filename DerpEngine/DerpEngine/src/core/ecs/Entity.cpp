#include "Entity.h"

#include "EntityManager.h"

namespace DERP {

	Entity::Entity(uint32_t _ID)
	{
		printf("Entity Created ID: %i\n", _ID);

		ID = _ID;
	}

	Entity::~Entity()
	{
		printf("Killed %s\n",name.c_str());

		//Removed by parent

		//Kill all children
		for (auto x : children) {
			delete x;
		}

		children.clear();
	}

	void Entity::addComponent(Component* comp)
	{
		componentList.insert( {comp, true } );
	}

	bool Entity::isComponent(Component* comp)
	{
		if (componentList.find(comp) == componentList.end()) {
			return false;
		}
		else {
			return true;
		}
	}

	Entity* Entity::getParent()
	{
		return parent;
	}

	std::vector<Entity*>& Entity::getChildren()
	{
		return children;
	}

	std::string Entity::getName()
	{
		return name;
	}

	void Entity::setName(std::string s)
	{
		name = s;
	}

	std::unordered_map<Component*, bool>& Entity::OLD_getComponentMap()
	{
		return componentList;
	}

}