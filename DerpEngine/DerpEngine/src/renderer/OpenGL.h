#pragma once

#include "RenderAPI.h"

namespace DERP {

	class OpenGL : public RenderAPI
	{
	private:
	public:
		void Render();
		void ClearScreen();
	};

}

