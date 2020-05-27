#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

namespace DERP 
{
	class Lights 
	{
	private:
		Shader* shaderManager;

		struct DirectionalLightData
		{
			glm::vec4 camera_position = glm::vec4(1, 0, 0, 1);
			glm::vec4 light_position = glm::vec4(0, 0, 1, 1);
			glm::vec4 light_diffuse = glm::vec4(1, 1, 1, 1);
		};

		DirectionalLightData dirLightData;
		unsigned int ubo = 0;

		glm::vec4 cameraPos;
	public:
		void InitLights(unsigned int shaderNum);

		void setCameraPosition(glm::vec3 position);

		void UpdateDirectionalLight(uint32_t shaderID);
		void UpdatePointLight(uint32_t shaderID);
	};
}