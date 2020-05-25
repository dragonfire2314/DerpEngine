#pragma once
#include "core.h"

#include "ecs/ComponentManager.h"

#include "../renderer/RenderAPI.h"
#include "input/Input.h"
#include "time/Time.h"

#include <stdio.h>

namespace DERP {

	class DERP_API Application
	{
	private:
	protected:
		void* window;
		RenderAPI* renderer;

		Input input;
		Time time;

		void start();
		ComponentManager componentManager;
		SystemManager systemManager;
	public:

		Application();
		~Application();

		//Starts the application and takes control of the program
		//Main game loop
		virtual void Run();

		void Init();
	};
}