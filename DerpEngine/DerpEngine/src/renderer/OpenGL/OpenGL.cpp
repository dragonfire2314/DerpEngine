#include "OpenGL.h"

#include "../../../external/glm/glm.hpp"
#include "../../../external/glm/gtc/quaternion.hpp"
#include "../../../external/glm/gtx/quaternion.hpp"

#include "../../core/ecs/components/ComponentMaterial.h"
#include "../../core/ecs/components/ComponentCamera.h"

#include <unordered_map>
#include <GL/glew.h>

#include <sstream>

namespace DERP {

	void OpenGL::Render()
	{
		RenderMainCamera();
		RenderNonMainCameras();
	}

	void OpenGL::RenderMainCamera() 
	{
		//Must render all mesh components
		std::unordered_map<uint32_t, Mesh*> data = ComponentMesh::getInstance()->getDataMap();

		//Loop though meshes
		for (auto x : data)
		{
			//Check if mesh is valid i.e. has a mesh, and has a material
			if (x.second->mesh == nullptr || !(EntityManager::getInstance().getEntity(x.first)->isComponent(ComponentMaterial::getInstance())))
			{
				//Can't render
				continue;
			}

			//Render
			//MVP generation
			// Model matrix : an identity matrix (model will be at the origin)
			View = glm::lookAt(
				((Transform*)ComponentTransform::getInstance()->getData(ComponentCamera::getInstance()->getMainCamera()))->position,
				glm::vec3(0, 0, 0),
				glm::vec3(0, 1, 0)
			);

			Transform* t = ComponentManager::getInstance()->
				getComponent<Transform>(ComponentTransform::getInstance(), EntityManager::getInstance().getEntity(x.first));

			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, t->position);
			trans = trans * glm::toMat4(t->rotation);
			trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));

			// Our ModelViewProjection : multiplication of our 3 matrices
			mvp = Projection * View * trans;

			GLuint MatrixID = glGetUniformLocation(shader.getShader(x.first), "MVP");
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUseProgram(shader.getShader(x.first));

			//Bind stuff
			//Vertex Buffer
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, VBManager.getVertexBuffer(x.first));
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
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBManager.getIndexBuffer(x.first));
			//Texture
			glBindTexture(GL_TEXTURE_2D, textureManagerGL.getTexture(x.first));
			glBindVertexArray(VertexArrayID);

			//Draw Call
			//glDrawArrays(GL_TRIANGLES, 0, x.second->mesh->Vertices.size());
			glDrawElements(
				GL_TRIANGLES,                      // mode
				x.second->mesh->Indices.size(),    // count
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
		VBManager.makeVertexBuffers();


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
		VBManager.updateVetexBuffer(entityID);
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