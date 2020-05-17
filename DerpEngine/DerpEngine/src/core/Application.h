#include "core.h"
#include "ecs/ComponentManager.h"

#include <stdio.h>

#ifdef DERP_DLL
	
#endif

namespace DERP {

	class DERP_API Application
	{
	private:
#ifdef DERP_DLL
		
#endif
	protected:
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