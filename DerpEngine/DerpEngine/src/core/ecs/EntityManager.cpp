#include "EntityManager.h"

#include "ComponentManager.h"
#include "Components.h"

namespace DERP 
{
	namespace EM
	{
		uint32_t counter;
		std::vector<Entity> entitys;
		//Key - EntityID
		//Value - Index to array
		std::unordered_map<uint32_t, uint32_t> idToEntity;

		void Init()
		{
			//Root Entity
			CreateEntity(-2);
		}

		uint32_t CreateEntity()
		{
			return CreateEntity(0);
		}
		uint32_t CreateEntity(uint32_t entityID)
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
		Entity* getEntity(uint32_t entityID)
		{
			if (idToEntity.count(entityID))
				return &entitys[idToEntity[entityID]];
			else
				return nullptr;
		}
		bool isComponent(uint32_t entityID, uint32_t componentID)
		{
			return entitys[idToEntity[entityID]].components[componentID];
		}
		void addComponent(uint32_t entityID, uint32_t componentID)
		{
			//TODO - assert that only one of each compoent exists on a entity
			entitys[idToEntity[entityID]].components[componentID] = 1;
		}
		std::bitset<UINT8_MAX> getSignature(uint32_t entityID)
		{
			return getEntity(entityID)->components;
		}
	}
}