#include "ComponentMaterial.h"

namespace DERP {

	uint32_t ComponentMaterial::addComponent(uint32_t entityID)
	{
		Material *mat = new Material();

		data.insert({ entityID, mat });

		return 0;
	}

	size_t* ComponentMaterial::getData(uint32_t entityID)
	{
		return (size_t*)(data[entityID]);
	}

	std::unordered_map<uint32_t, Material*> ComponentMaterial::getDataMap()
	{
		return data;
	}
}