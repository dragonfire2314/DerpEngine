#pragma once
#include "core.h"

#include "ecs/ComponentManager.h"

#include "../renderer/RenderAPI.h"
#include "input/Input.h"
#include "time/Time.h"
#include "physics/Physics.h"

#include <stdio.h>

namespace DERP {

	class DERP_API Application
	{
	private:
	protected:
		void* window;
		RenderAPI* renderer;

		//Physics physics;

		Input input;

		void start();
		SystemManager systemManager;

		uint32_t screenWidth;
		uint32_t screenHeight;

		bool shouldUpdateRes = true;
	public:

		Application();
		~Application();

		//Starts the application and takes control of the program
		//Main game loop
		virtual void Run();

		void Init(uint32_t width, uint32_t height);

		void UpdateResolution();
	};
}