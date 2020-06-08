#pragma once
#include "../core/Application.h"
#include "../core/TextureManager.h"

namespace DERP {

	class DERP_API ApplicationEditor : public Application
	{
	private:
		void (*updateFunc)(void*);
	public:
		

		ApplicationEditor();
		~ApplicationEditor();

		//Starts the application and takes control of the program
		//Main game loop
		void startUp();
		virtual void Update3();
		virtual void Start3();
		void Run();

		void setUpdate(void (*func)(void*));

		uint32_t getHeight();
		uint32_t getWidth();

		void setHeight(uint32_t height);
		void setWidth(uint32_t width);

		RenderAPI* getRenderAPI();

		TextureManager textureManager;
	};

}
