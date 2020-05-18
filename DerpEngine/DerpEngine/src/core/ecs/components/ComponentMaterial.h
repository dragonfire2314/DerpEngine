#pragma once

#include "../Component.h"

#include "ComponentStructs.h"

#include <unordered_map>

namespace DERP {

	class DERP_API ComponentMaterial : public Component
	{
	private:
		//Key - Entity ID
		//Value - Transform_Data
		std::unordered_map<uint32_t, Material*> data;

	public:
		//Basic singleton method
		static ComponentMaterial* getInstance()
		{
			static ComponentMaterial instance;
			return &instance;
		}

		//C++ 11 something?
		ComponentMaterial(ComponentMaterial const&) = delete;
		void operator=(ComponentMaterial const&) = delete;

		ComponentMaterial() {}
		~ComponentMaterial() {}

		uint32_t addComponent(uint32_t entityID);
		size_t* getData(uint32_t entityID);

		//Custom
		std::unordered_map<uint32_t, Material*>* getDataMap();
	};

}