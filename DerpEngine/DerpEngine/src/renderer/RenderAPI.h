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

		virtual void updateMesh(uint32_t entityID) = 0;
		virtual void updateShader(uint32_t entityID) = 0;
		virtual void updateTexture(uint32_t entityID) = 0;
	};

}