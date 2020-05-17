#pragma once

#include "../core.h"

#include "Entity.h"
#include "components/Script.h"
#include "components/ComponentScript.h"

namespace DERP {

	class DERP_API ComponentManager
	{
	private:

	public:
		ComponentManager();
		~ComponentManager();

		//C++ 11 something?
		ComponentManager(ComponentManager const&) = delete;
		void operator=(ComponentManager const&)   = delete;

		void addComponent(Component* comp, Entity* e);
		void addScript(Script* script, Entity* e);

		template <typename T>
		T* getComponent(Component* comp, Entity* e)
		{
			//Check if component exsists on enitiy making the call
			if (e->isComponent(comp))
			{
				return (T*)comp->getData(e->getID());
			}

			return nullptr;
		}
	};

}

