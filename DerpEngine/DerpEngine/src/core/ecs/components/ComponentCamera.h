#pragma once

#include "../Component.h"

#include "ComponentStructs.h"

#include <unordered_map>

namespace DERP {

	class DERP_API ComponentCamera : public Component
	{
	private:
		//Key - Entity ID
		//Value - Camera
		std::unordered_map<uint32_t, Camera> data;

		static uint32_t mainCamera;
	public:
		//Basic singleton method
		static ComponentCamera* getInstance()
		{
			static ComponentCamera instance;
			return &instance;
		}

		//C++ 11 something?
		ComponentCamera(ComponentCamera const&) = delete;
		void operator=(ComponentCamera const&) = delete;

		ComponentCamera() {}
		~ComponentCamera() {}

		uint32_t addComponent(uint32_t entityID);
		size_t* getData(uint32_t entityID);

		void setMainCamera(uint32_t entityID);
		uint32_t getMainCamera();
	};

}
