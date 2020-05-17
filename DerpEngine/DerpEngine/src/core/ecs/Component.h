#pragma once
#include "../core.h"

#include <stdint.h>

namespace DERP {

	class DERP_API Component
	{
	private:
		
	public:
		Component();
		~Component();

		virtual uint32_t addComponent(uint32_t entityID) { return 0; }
		virtual void updateComponent() {}
		virtual void startComponent() {}

		virtual size_t* getData(uint32_t entityID) { return nullptr; }
	};

}


