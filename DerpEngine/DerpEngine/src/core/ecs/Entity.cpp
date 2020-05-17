#include "Entity.h"

namespace DERP {

	Entity::Entity(uint32_t _ID)
	{
		printf("Entity Created ID: %i\n", _ID);

		ID = _ID;
	}

	Entity::~Entity()
	{

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

}