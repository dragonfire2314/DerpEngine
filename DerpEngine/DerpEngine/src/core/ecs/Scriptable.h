#pragma once

#include "../core.h"

#include <stdint.h>

#include "ComponentManager.h"
#include "../input/Input.h"
#include "../time/Time.h"

namespace DERP {

		class DERP_API Scriptable
		{
			friend class Application;
			friend class ApplicationEditor;
		private:
			uint32_t entity = -1;
		protected:
			template <typename T>
			T* getComponent()
			{
				return ComponentManager::GetComponent<T>(entity);
			}

			Time time;
		public:
			Scriptable() {}
			~Scriptable() {}

			//Script functions
			virtual void Update() {};
			virtual void Start() {};
		};

}