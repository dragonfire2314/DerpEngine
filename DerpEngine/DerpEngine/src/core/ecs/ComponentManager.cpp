#include "ComponentManager.h"

namespace DERP 
{
	namespace CM
	{
		//Key - Component name
		//Value - Component ID
		std::unordered_map<const char*, uint8_t> nameToID;
		//Key - Component name
		//Value - ComponentArray
		std::unordered_map<const char*, ComponentBase*> nameToComponent;
		uint32_t componentID = 0;
	}
}
