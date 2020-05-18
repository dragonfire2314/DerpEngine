#pragma once

#include "../RenderAPI.h"

#include "../../../external/glm/gtc/matrix_transform.hpp"
#include "../../core/ecs/components/ComponentMesh.h"

#include "Shader.h"
#include "VertexBufferManger.h"

#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/components/Transform.h"
#include "../../core/ecs/EntityManager.h"

namespace DERP {

	class OpenGL : public RenderAPI
	{
	private:
		Shader shader;
		VertexBufferManger VBManager;

		glm::mat4 mvp;
		glm::mat4 Projection;
		glm::mat4 View;
	public:
		void Render();
		void ClearScreen();
		void SetUp();
	};

}

