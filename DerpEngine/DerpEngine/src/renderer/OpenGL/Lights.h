#pragma once

#include "Shader.h"

namespace DERP 
{
	class Lights 
	{
	private:
		Shader* shaderManager;
	public:
		void InitLights(Shader* shader);


		void UpdateDirectionalLight(uint32_t shaderID);
		void UpdatePointLight(uint32_t shaderID);
	};
}