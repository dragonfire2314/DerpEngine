#include "OpenGL.h"

#include "../../../external/glm/glm.hpp"
#include "../../../external/glm/gtc/quaternion.hpp"
#include "../../../external/glm/gtx/quaternion.hpp"

#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/systems/System.h"

#include <unordered_map>
#include <GL/glew.h>

#include <sstream>

namespace DERP {

	void OpenGL::Render()
	{
		for (auto x : sys_cameras->Entities) { //Render screen once per camera
			//TODO - Allow cameras to choose render method (screen space, or texture)
			//Setup veiw matrix before call
			Transform* t = ComponentManager::GetComponent<Transform>(x);
			glm::vec3 euler = glm::eulerAngles(t->rotation) * 180.0f / 3.14159f;
			View = glm::lookAt(t->position, t->position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

			RenderMainCamera();
		}
	}

	void OpenGL::RenderMainCamera() 
	{
		//Must render all mesh components

		//Loop though meshes
		for (auto x : sys_renderer->Entities)
		{
			Mesh* mesh = ComponentManager::GetComponent<Mesh>(x);

			//Check if mesh is valid
			if (mesh->mesh == nullptr) continue;

			Transform* t = ComponentManager::GetComponent<Transform>(x);

			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, t->position);
			trans = trans * glm::toMat4(t->rotation);
			trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));

			// Our ModelViewProjection : multiplication of our 3 matrices
			mvp = Projection * View * trans;

			GLuint MatrixID = glGetUniformLocation(shader.getShader(x), "MVP");
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUseProgram(shader.getShader(x));

			//Bind stuff
			//Vertex Buffer
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, VBManager->getVertexBuffer(x));
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			//Normals
			glEnableVertexAttribArray(1);
			//glBindBuffer(GL_ARRAY_BUFFER, VBManager.getVertexBuffer(x.first));
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			//Tex Coord
			glEnableVertexAttribArray(2);
			//glBindBuffer(GL_ARRAY_BUFFER, VBManager.getVertexBuffer(x.first));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			//Index Buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBManager->getIndexBuffer(x));
			//Texture
			//glBindTexture(GL_TEXTURE_2D, textureManagerGL.getTexture(x));
			//glBindVertexArray(VertexArrayID);

			//Draw Call
			//glDrawArrays(GL_TRIANGLES, 0, x.second->mesh->Vertices.size());
			glDrawElements(
				GL_TRIANGLES,                      // mode
				mesh->mesh->Indices.size(),        // count
				GL_UNSIGNED_INT,                   // type
				(void*)0                           // element array buffer offset
			);
			//Unbind stuff
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}
	}

	void OpenGL::RenderNonMainCameras() 
	{
		//GLuint FramebufferName = 0;
		//glGenFramebuffers(1, &FramebufferName);
		//glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	}

	void OpenGL::ClearScreen()
	{
		glClearColor(0.4, 0.2, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL::SetUp()
	{
		VBManager = new VertexBufferManger();

		/*******************
		*	Depth
		********************/
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		/*******************
		*	VBA and shaders
		********************/
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		//Load the shaders
		shader.loadShaders();
		//Load the vertexBuffers
		VBManager->makeVertexBuffers();


		/*******************
		*	Projection and Camera
		********************/
		Projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)768, 0.1f, 100.0f);

		View = glm::lookAt(
			glm::vec3(0, 0, 4),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0) 
		);

		/*******************
		*	Texture
		********************/
		//Wrap mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//Scaling filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//Mip map img filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//Load all textures that are needed
		textureManagerGL.LoadTextures();
	}

	void OpenGL::updateMesh(uint32_t entityID)
	{
		VBManager->updateVetexBuffer(entityID);
	}

	void OpenGL::updateShader(uint32_t entityID)
	{
		shader.updateShader(entityID);
		//textureManagerGL.LoadTextures();
	}

	void OpenGL::updateTexture(uint32_t entityID) 
	{
		textureManagerGL.updateTexture(entityID);
	}

}