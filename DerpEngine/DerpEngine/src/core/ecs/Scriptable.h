#pragma once

#include "../core.h"

#include <stdint.h>
#include <vector>

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
				return CM::GetComponent<T>(entity);
			}
			template <typename T>
			std::vector<T*> getComponentInChildern() 
			{
				std::vector<T*> ret;

				for (auto x : EM::getEntity(entity)->childern) 
				{
					if (CM::IsComponent<T>(x))
						ret.push_back(CM::GetComponent<T>(x));
				}

				return ret;
			}
		public:
			Scriptable() {}
			~Scriptable() {}

			//Script functions
			virtual void Update() {};
			virtual void Start() {};
		};

}