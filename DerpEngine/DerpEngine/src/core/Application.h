#include "core.h"
#include "ecs/ComponentManager.h"

#include <stdio.h>

namespace DERP {

	class DERP_API Application
	{
	protected:
		inline ComponentScript* getCS() { return ComponentScript::getInstance(); }
	public:

		Application();
		~Application();

		void test();

		//Starts the application and takes control of the program
		//Main game loop
		virtual void Run();
	};
}