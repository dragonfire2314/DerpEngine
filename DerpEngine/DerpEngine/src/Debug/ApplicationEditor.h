#pragma once
#include "../core/Application.h"
#include "../core/TextureManager.h"

namespace DERP {

	class DERP_API ApplicationEditor : public Application
	{
	private:
		void (*updateFunc)();
	public:
		

		ApplicationEditor();
		~ApplicationEditor();

		//Starts the application and takes control of the program
		//Main game loop
		void startUp();
		virtual void Update3();
		virtual void Start3();
		void Run();

		void setUpdate(void (*func)());

		RenderAPI* getRenderAPI();

		TextureManager textureManager;
	};

}
