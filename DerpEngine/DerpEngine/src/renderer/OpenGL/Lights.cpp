#include "Lights.h"

#include "../../core/ecs/systems/System.h"
#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/Components.h"

#include <GL/glew.h>

namespace DERP 
{
	void Lights::InitLights(Shader* shader)
	{
		shaderManager = shader;
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
				Transform* l = ComponentManager::GetComponent<Transform>(x);
				GLuint dirID = glGetUniformLocation(shaderID, "LightPos");
				glUniform3f(dirID, l->position.x, l->position.y, l->position.z);

				printf("r: %f, g: %f, b: %f, dir: %i\n", l->position.x, l->position.y, l->position.z, dirID);

				//printf("Debug");
			}
	}

}