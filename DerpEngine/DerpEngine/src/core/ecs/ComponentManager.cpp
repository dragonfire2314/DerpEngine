#include "ComponentManager.h"

namespace DERP 
{
	std::unordered_map<const char*, uint8_t> ComponentManager::nameToID;
	std::unordered_map<const char*, ComponentBase*> ComponentManager::nameToComponent;
	uint32_t ComponentManager::componentID = 0;

	ComponentManager::ComponentManager() {}
	ComponentManager::~ComponentManager() {}
}
