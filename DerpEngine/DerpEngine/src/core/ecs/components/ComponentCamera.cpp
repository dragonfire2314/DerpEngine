#include "ComponentCamera.h"

namespace DERP {

	uint32_t ComponentCamera::mainCamera;

	uint32_t ComponentCamera::addComponent(uint32_t entityID)
	{
		data.insert({ entityID, Camera() });

		return 0;
	}

	size_t* ComponentCamera::getData(uint32_t entityID)
	{
		return (size_t*)(&(data[entityID]));
	}

	void ComponentCamera::setMainCamera(uint32_t entityID)
	{
		mainCamera = entityID;
	}

	uint32_t ComponentCamera::getMainCamera()
	{
		return mainCamera;
	}

}