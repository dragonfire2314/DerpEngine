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


		//Assign entity to the root
		root.push_back(newEntity);
		//Give the entiy a nullptr for parnet since its parent is root
		newEntity->parent = nullptr;


		//Return the enity made
		return newEntity;
	}

	Entity* EntityManager::createEntity(Entity* parent)
	{
		//Create a new entity on heap
		Entity* newEntity = new Entity(counter);
		//Inster pointer into the map
		entityMap.insert({ counter, newEntity });
		//Increament entity ID counter
		counter++;

		//Assign parent
		newEntity->parent = parent;
		//Add child to parent object
		parent->children.push_back(newEntity);

		//Return the enity made
		return newEntity;
	}

	std::vector<Entity*>& EntityManager::getRoot()
	{
		return root;
	}

	void EntityManager::removeFromRoot(Entity* e)
	{
		
	}

	void EntityManager::removeEntity(Entity* e)
	{
		//Remove from vector
		std::vector<Entity*>::iterator toErease;
		toErease = std::find(root.begin(), root.end(), e);
		if (toErease != root.end())
		{
			root.erase(toErease);
		}
		//Remove from map
		entityMap.erase(e->getID());
		//Delete
		delete e;
	}

}