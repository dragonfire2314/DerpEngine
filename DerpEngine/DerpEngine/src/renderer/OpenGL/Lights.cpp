#include "Lights.h"

#include "../../core/ecs/systems/System.h"
#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/Components.h"

#include <GL/glew.h>

namespace DERP 
{
	void Lights::InitLights(unsigned int shaderNum)
	{
		//shaderManager = shader;

		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionalLightData), &dirLightData, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		unsigned int lights_index = glGetUniformBlockIndex(shaderNum, "dirLight");
		glUniformBlockBinding(shaderNum, lights_index, 2);

		glBindBufferBase(GL_UNIFORM_BUFFER, 2, ubo);
	}

	void Lights::setCameraPosition(glm::vec3 position)
	{
		cameraPos = glm::vec4(position, 1.0);
	}

	void Lights::UpdateDirectionalLight(uint32_t shaderID)
	{
		//for (auto x : sys_dirLight->Entities) 
		//{
		//	//GLuint MatrixID = glGetUniformLocation(shader.getShader(x), "MVP");
		//	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		//	DirectionalLight* l = ComponentManager::GetComponent<DirectionalLight>(x);
		//	GLuint dirID = glGetUniformLocation(shaderID, "dirLight");
		//	glUniform3f(dirID, l->direction.x, l->direction.y, l->direction.z);

		//	//printf("r: %f, g: %f, b: %f, dir: %i\n", l->direction.x, l->direction.y, l->direction.z, dirID);

		//	//printf("Debug");
		//}
	}

	void Lights::UpdatePointLight(uint32_t shaderID)
	{
		for (auto x : sys_dirLight->Entities)
		{
			//Update light data
			Transform* t = ComponentManager::GetComponent<Transform>(x);

			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &cameraPos[0]);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &t->position[0]);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 2, sizeof(glm::vec4), &dirLightData.light_diffuse[0]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	}

}