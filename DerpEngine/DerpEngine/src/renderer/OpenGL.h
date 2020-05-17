#pragma once

#include "RenderAPI.h"

#include "../core/ecs/components/ComponentMesh.h"

namespace DERP {

	class OpenGL : public RenderAPI
	{
	private:
	public:
		void Render();
		void ClearScreen();
	};

}

