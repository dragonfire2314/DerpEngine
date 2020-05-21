#pragma once
#include "core.h"

#include "ecs/ComponentManager.h"

#include "../renderer/RenderAPI.h"

#include <stdio.h>

namespace DERP {

	class DERP_API Application
	{
	private:
	protected:
		void* window;
		RenderAPI* renderer;

		inline ComponentScript* getCS() { return ComponentScript::getInstance(); }
		void start();
	public:

		Application();
		~Application();

		

		//Starts the application and takes control of the program
		//Main game loop
		virtual void Run();
	};
}