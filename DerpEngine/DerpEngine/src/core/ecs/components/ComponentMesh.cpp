#include "ComponentMesh.h"

namespace DERP {

	uint32_t ComponentMesh::addComponent(uint32_t entityID)
	{
		//data.insert({ entityID, Transform() });
		return 0;
	}

	size_t* DERP::ComponentMesh::getData(uint32_t entityID)
	{
		return (size_t*)(&(data[entityID]));
	}

}