#include "EntityManager.h"

#include "ComponentManager.h"
#include "Components.h"

namespace DERP 
{
	uint32_t EntityManager::counter = 0;
	std::unordered_map<uint32_t, uint32_t> EntityManager::idToEntity;
	std::vector<Entity> EntityManager::entitys;

	void EntityManager::Init()
	{
		//Root Entity
		CreateEntity(-2);
	}

	uint32_t EntityManager::CreateEntity()
	{
		return CreateEntity(0);
	}
	uint32_t EntityManager::CreateEntity(uint32_t entityID)
	{
		if (entityID == -2)
		{
			//Create root entity
			printf("Entity Created Root: %i\n", 0);

			Entity ent;
			ent.name = "Root Entity";
			ent.ID = 0;
			ent.parent = nullptr;

			entitys.push_back(ent);

			idToEntity.insert({ 0 , 0 });

			counter++;
			return 0;
		}
		else 
		{
			printf("Entity Created %i\n", counter);

			Entity ent;
			ent.name = "Entity: " + std::to_string(counter);
			ent.ID = counter;
			ent.parent = getEntity(entityID);

			entitys.push_back(ent);

			//Add to parents children list
			getEntity(entityID)->childern.push_back(ent.ID);

			idToEntity.insert({ counter , counter });

			counter++;
			return counter - 1;
		}
	}
	Entity* EntityManager::getEntity(uint32_t entityID)
	{
		return &entitys[idToEntity[entityID]];
	}
	bool EntityManager::isComponent(uint32_t entityID, uint32_t componentID)
	{
		return entitys[idToEntity[entityID]].components[componentID];
	}
	void EntityManager::addComponent(uint32_t entityID, uint32_t componentID)
	{
		entitys[idToEntity[entityID]].components[componentID] = 1;
	}
	std::bitset<UINT8_MAX> EntityManager::getSignature(uint32_t entityID)
	{
		return getEntity(entityID)->components;
	}
}