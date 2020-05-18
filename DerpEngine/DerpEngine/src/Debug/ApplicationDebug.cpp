#include "ApplicationDebug.h"

#include "../core/ecs/components/Transform.h"

namespace DERP {

	ApplicationDebug::ApplicationDebug()
	{

	}

	ApplicationDebug::~ApplicationDebug()
	{

	}

	void ApplicationDebug::Run() 
	{
		//Engine setup
		getCS()->startComponent();

		//Engine loop
		do {
			//Run physics


			//ComponentTransform::getInstance()->printTransforms();

			//Run scripts
			getCS()->updateComponent();

			//Render



		} while (1);
	}

}