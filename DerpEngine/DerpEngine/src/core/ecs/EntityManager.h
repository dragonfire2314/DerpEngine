#pragma once

#include <unordered_map>
#include <vector>

#include "../core.h"
#include "Entity.h"

namespace DERP 
{
	class DERP_API EntityManager 
	{
	private:
		static uint32_t counter;
		static std::vector<Entity> entitys;
		//Key - EntityID
		//Value - Index to array
		static std::unordered_map<uint32_t, uint32_t> idToEntity;
	public:
		static void Init();
		static uint32_t CreateEntity();
		static uint32_t CreateEntity(uint32_t entityID);
		static Entity* getEntity(uint32_t entityID);

		static bool isComponent(uint32_t entityID, uint32_t componentID);
		static void addComponent(uint32_t entityID, uint32_t componentID);

		static std::bitset<UINT8_MAX> getSignature(uint32_t entityID);
	};
}