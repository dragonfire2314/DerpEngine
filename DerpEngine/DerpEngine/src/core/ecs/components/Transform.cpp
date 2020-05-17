#include "Transform.h"

namespace DERP {

	uint32_t ComponentTransform::addComponent(uint32_t entityID)
	{
		data.insert({entityID, Transform()});

		return 0;
	}

	size_t* ComponentTransform::getData(uint32_t entityID)
	{
		return (size_t*)(&(data[entityID]));
	}

}
