#pragma once

#include "../core.h"

#include <stdint.h>

#include "ComponentManager.h"

namespace DERP {

		class DERP_API Scriptable
		{
			friend class Application;
		private:
			uint32_t entity = -1;
		protected:
			template <typename T>
			T* getComponent()
			{
				return ComponentManager::GetComponent<T>(entity);
			}
		public:
			Scriptable() {}
			~Scriptable() {}

			//Script functions
			virtual void Update() {};
			virtual void Start() {};
		};

}