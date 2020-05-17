#include "EntityManager.h"

namespace DERP {

	EntityManager::EntityManager() 
	{
		counter = 0;
	}

	EntityManager::~EntityManager() 
	{

	}

	Entity* DERP::EntityManager::getEntity(uint32_t ID)
	{
		return entityMap[ID];
	}

	//TODO - Entity ID reusal
	Entity* EntityManager::createEntity()
	{
		//Create a new entity on heap
		Entity* newEntity = new Entity(counter);
		//Inster pointer into the map
		entityMap.insert({ counter, newEntity });
		//Increament entity ID counter
		counter++;
		//Return the enity made
		return newEntity;
	}

}