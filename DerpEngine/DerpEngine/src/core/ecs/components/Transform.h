#pragma once

#include "../Component.h"

#include "ComponentStructs.h"

#include <unordered_map>

namespace DERP {

	class DERP_API ComponentTransform : public Component
	{
	private:
		//Key - Entity ID
		//Value - Transform_Data
		std::unordered_map<uint32_t, Transform> data;

	public:
		//Basic singleton method
		static ComponentTransform* getInstance()
		{
			static ComponentTransform instance;
			return &instance;
		}

		//C++ 11 something?
		ComponentTransform(ComponentTransform const&)      = delete;
		void operator=(ComponentTransform const&) = delete;

		ComponentTransform() {}
		~ComponentTransform() {}

		uint32_t addComponent(uint32_t entityID);
		size_t* getData(uint32_t entityID);

		//Debug functions
		void printTransforms() 
		{
			printf("Transforms\n");

			for (auto const& x : data)
			{
				printf("Entity: %i Position: %f, %f, %f\n", x.first, x.second.position.x, x.second.position.y, x.second.position.z);
			}

			printf("End\n");
		}
	};

}
