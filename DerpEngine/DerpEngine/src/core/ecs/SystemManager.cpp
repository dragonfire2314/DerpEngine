#include "SystemManager.h"

namespace DERP
{
	std::unordered_map<const char*, std::bitset<UINT8_MAX>> SystemManager::Signatures;
	std::unordered_map<const char*, System*> SystemManager::Systems;
}