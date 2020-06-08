#pragma once

#include <stdint.h>

namespace DERP {

	//Abstract class that is overrided by a graphics class
	//Example openGL class
	class RenderAPI
	{
	private:
	public:
		virtual void Render() = 0;
		virtual void ClearScreen() = 0;
		virtual void SetUp() = 0;
		virtual void* RenderToTexture() = 0; //Returns pointer to texture ID? //Like this for other API's to hopefully work

		virtual void updateMesh(uint32_t entityID) = 0;
		virtual void updateShader(uint32_t entityID) = 0;
		virtual void updateTexture(uint32_t entityID) = 0;
		virtual void updateFrameBuffer() = 0;

		uint32_t renderWidth;
		uint32_t renderHeight;

		uint32_t mainCamera;
	};

}