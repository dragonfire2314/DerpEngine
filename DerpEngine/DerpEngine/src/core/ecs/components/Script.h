#pragma once

#include "../../core.h"
#include "../Entity.h"

namespace DERP {

	class DERP_API Script
	{
	friend class ComponentScript;

	private:
		Entity* entity = nullptr;
	protected:
		template <typename T>
		T* getComponent(Component* comp) 
		{
			//Check if component exsists on enitiy making the call
			if (entity->isComponent(comp))
			{
				return (T*)comp->getData(entity->getID());
			}

			return nullptr;
		}
	public:
		Script() {}
		~Script() {}

		//Script functions
		virtual void Update() {};
		virtual void Start() {};
	};

}