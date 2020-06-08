#pragma once

#include "../RenderAPI.h"

#include "../../../external/glm/gtc/matrix_transform.hpp"

//#include "Shader.h"
#include "VertexBufferManger.h"
#include "TextureManagerGL.h"
#include "Lights.h"

#include "../../core/ecs/Components.h"

#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/EntityManager.h"

namespace DERP {

	class OpenGL : public RenderAPI
	{
	private:
		Shader shader;
		VertexBufferManger* VBManager;
		TextureManagerGL textureManagerGL;
		Lights light;

		GLuint VertexArrayID;

		glm::mat4 mvp;
		glm::mat4 Projection;
		glm::mat4 View;

		GLuint FramebufferName;
		GLuint renderedTexture;
		GLuint depthrenderbuffer;

		GLuint postQuadVerts;
		GLuint postQuadInds;

		//Temp
		glm::vec3 mainCamPos;

		//Functions
		void setupFrameBuffer();
		void renderFrameBufferToScreen();
		void setupCameraForFrame(uint32_t camera);
	public:
		void Render();
		void ClearScreen();
		void SetUp();
		void* RenderToTexture();

		//Returns texture ID
		void RenderMainCamera(Entity* baseEntity);

		void updateMesh(uint32_t entityID);
		void updateShader(uint32_t entityID);
		void updateTexture(uint32_t entityID);
		void updateFrameBuffer();
	};

}

