#include "EntityManager.h"

namespace DERP {

	EntityManager::EntityManager() 
	{

	}

	EntityManager::~EntityManager() 
	{

	}

	//TODO - Entity ID reusal
	Entity* EntityManager::createEntity()
	{
		//Create a new entity on heap
		Entity* newEntity = new Entity();
		//Inster pointer into the map
		entityMap.insert({ counter, newEntity });
		//Increament entity ID counter
		counter++;
		//Return the enity made
		return newEntity;
	}

}