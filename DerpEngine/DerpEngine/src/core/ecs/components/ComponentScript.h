#pragma once
#include "../../core.h"

#include "../Component.h"
#include "Script.h"

#include <unordered_map>

namespace DERP {

	class ComponentScript : public Component
	{
	public:

		//Basic singleton method
		static ComponentScript* getInstance()
		{
			static ComponentScript instance;
			return &instance;
		}

		//C++ 11 something?
		ComponentScript(ComponentScript const&) = delete;
		void operator=(ComponentScript const&) = delete;

		ComponentScript() {}
		~ComponentScript() {}

		uint32_t addComponent(uint32_t entityID);
		uint32_t addScript(uint32_t entityID, Script* script);
		void updateComponent();
		void startComponent();

	private:
		//Key - Entity ID
		//Value - Sript
		std::unordered_map<uint32_t, Script*> data;
	};

}

