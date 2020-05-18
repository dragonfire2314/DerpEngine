#include "OpenGL.h"

#include "../../../external/glm/glm.hpp"
#include "../../../external/glm/gtc/quaternion.hpp"
#include "../../../external/glm/gtx/quaternion.hpp"

#include <unordered_map>
#include <GL/glew.h>

#include <sstream>

namespace DERP {

	void OpenGL::Render()
	{
		printf("Render\n");

		//Must render all mesh components
		std::unordered_map<uint32_t, Mesh*> data = ComponentMesh::getInstance()->getDataMap();

		//Loop though meshes
		for (auto x : data) 
		{
			//Render

			//MVP generation
			// Model matrix : an identity matrix (model will be at the origin)
			Transform* t = ComponentManager::getInstance()->
				getComponent<Transform>(ComponentTransform::getInstance(), EntityManager::getInstance().getEntity(x.first));
			
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, t->position);
			trans = trans * glm::toMat4(t->rotation);
			trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));

			

			// Our ModelViewProjection : multiplication of our 3 matrices
			mvp = Projection * View * trans; // Remember, matrix multiplication is the other way around

			GLuint MatrixID = glGetUniformLocation(shader.getShader(0), "MVP");

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

			//Load Shader
			glUseProgram(shader.getShader(0));

			//Bind stuff
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, VBManager.getVertexBuffer(x.first));

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			//Draw Call
			glDrawArrays(GL_TRIANGLES, 0, x.second->mesh->Vertices.size());
			//Unbind stuff
			glDisableVertexAttribArray(0);
		}

	}

	void OpenGL::ClearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL::SetUp()
	{
		//Set up this thing
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		//Load the shaders
		shader.loadShaders();
		//Load the vertexBuffers
		VBManager.makeVertexBuffers();

		//Projection
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)768, 0.1f, 100.0f);

		// Camera matrix
		View = glm::lookAt(
			glm::vec3(0, 0, 4), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}

}