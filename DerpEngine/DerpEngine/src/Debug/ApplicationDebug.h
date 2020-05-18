#pragma once
#include "../core/Application.h"

namespace DERP {

	class DERP_API ApplicationDebug : public Application
	{
	private:

	public:

		ApplicationDebug();
		~ApplicationDebug();

		//Starts the application and takes control of the program
		//Main game loop
		void Run();
	};

}

