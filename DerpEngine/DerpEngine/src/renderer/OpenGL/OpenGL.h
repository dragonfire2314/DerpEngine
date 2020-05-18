#pragma once

#include "../RenderAPI.h"

#include "../../core/ecs/components/ComponentMesh.h"

#include "Shader.h"

namespace DERP {

	class OpenGL : public RenderAPI
	{
	private:
		Shader shader;
	public:
		void Render();
		void ClearScreen();
		void LoadShader();
	};

}

