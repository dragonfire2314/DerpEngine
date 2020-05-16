#include "Entity.h"

namespace DERP {

	Entity::Entity()
	{
		printf("Entity Created\n");
	}

	Entity::~Entity()
	{

	}

	void Entity::addComponent(Component comp, uint32_t index)
	{
		//componentList.insert( {comp, comp } );
	}

}