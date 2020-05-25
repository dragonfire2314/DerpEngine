#include "VertexBufferManger.h"

#include "../../core/ecs/systems/System.h"
#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/EntityManager.h"

namespace DERP {

	void VertexBufferManger::makeVertexBuffers()
	{
		//TODO - Reuse a vertexbuffer when the mesh was already loaded

		//Loop though meshes
		for (auto x : sys_vertex->Entities) 
		{
			Mesh* m = ComponentManager::GetComponent<Mesh>(x);

			//Checks if a mesh is applied
			if (m->mesh == nullptr) continue;
				
			//Generate VBO for each mesh
			//Create an vector of data
			GLuint vertexbuffer;
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, m->mesh->Vertices.size() * sizeof(objl::Vertex),
				m->mesh->Vertices.data(), GL_STATIC_DRAW);
			//Store in mesh of object for fast look up
			m->VertexID = vertexbuffer;
			//Inster to list of buffers
			vertexBuffers.insert({ x, vertexbuffer });
			

			//Generate VBO indexes
			GLuint indexbuffer;
			glGenBuffers(1, &indexbuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->mesh->Indices.size() * sizeof(unsigned int),
				m->mesh->Indices.data(), GL_STATIC_DRAW);
			//Store in mesh of object for fast look up
			//Inster to list of buffers
			indexBuffers.insert({ x, indexbuffer });
		}
	}

	void VertexBufferManger::updateVetexBuffer(uint32_t entityID)
	{
		printf("Buffer Change Entity: %s\n", EntityManager::getEntity(entityID)->name.c_str());

		//Check for vertex buffer
		if (vertexBuffers.find(entityID) == vertexBuffers.end()) {
			//Create a new one
			vertexBuffers.insert({ entityID, 0 }); //Temp data here, will be replaced at end of function
			indexBuffers.insert({ entityID, 0 }); //Temp data here, will be replaced at end of function
		}
		else {
			//Destory the current
			glDeleteBuffers(1, &(vertexBuffers[entityID]));
			glDeleteBuffers(1, &(indexBuffers[entityID]));
		}

		Mesh* m = ComponentManager::GetComponent<Mesh>(entityID);

		//Create buffers for entity
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, m->mesh->Vertices.size() * sizeof(objl::Vertex),
			m->mesh->Vertices.data(), GL_STATIC_DRAW);
		//Store in mesh of object for fast look up
		m->VertexID = vertexbuffer;
		//Inster to list of buffers
		vertexBuffers[entityID] = vertexbuffer;

		//Generate VBO indexes
		GLuint indexbuffer;
		glGenBuffers(1, &indexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->mesh->Indices.size() * sizeof(unsigned int),
			m->mesh->Indices.data(), GL_STATIC_DRAW);
		indexBuffers[entityID] = indexbuffer;
	}

	GLuint VertexBufferManger::getIndexBuffer(uint32_t entityID) 
	{
		if (indexBuffers.find(entityID) == indexBuffers.end()) {
			return NULL;
		}
		else {
			return indexBuffers[entityID];
		}
	}

	GLuint VertexBufferManger::getVertexBuffer(uint32_t entityID) 
	{
		if (vertexBuffers.find(entityID) == vertexBuffers.end()) {
			return NULL;
		}
		else {
			return vertexBuffers[entityID];
		}
	}

}