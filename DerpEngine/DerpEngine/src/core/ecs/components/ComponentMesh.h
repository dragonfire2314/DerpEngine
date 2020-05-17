#pragma once

#include "../../core.h"
#include "../Component.h"

#include <unordered_map>

#include "ComponentStructs.h"

namespace DERP {

	class DERP_API ComponentMesh : public Component
	{
	private:
		//Key - Entity ID
		//Value - Transform_Data
		std::unordered_map<uint32_t, Mesh*> data;
	public:
		//Basic singleton method
		static ComponentMesh* getInstance()
		{
			static ComponentMesh instance;
			return &instance;
		}

		//C++ 11 something?
		ComponentMesh(ComponentMesh const&)  = delete;
		void operator=(ComponentMesh const&) = delete;

		ComponentMesh() {}
		~ComponentMesh() {}

		uint32_t addComponent(uint32_t entityID);
		size_t* getData(uint32_t entityID);
	};

}