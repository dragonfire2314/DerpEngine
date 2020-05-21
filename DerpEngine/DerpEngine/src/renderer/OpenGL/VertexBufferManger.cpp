#include "VertexBufferManger.h"

namespace DERP {

	void VertexBufferManger::makeVertexBuffers()
	{
		//TODO - Reuse a vertexbuffer when the mesh was already loaded

		//Get mesh list
		std::unordered_map<uint32_t, Mesh*> data = ComponentMesh::getInstance()->getDataMap();

		//Loop though meshes
		for (auto x : data) 
		{
			//Checks if a mesh is applied
			if (x.second->mesh == nullptr) continue;
				
			//Generate VBO for each mesh
			//Create an vector of data
			GLuint vertexbuffer;
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, x.second->mesh->Vertices.size() * sizeof(objl::Vertex),
				x.second->mesh->Vertices.data(), GL_STATIC_DRAW);
			//Store in mesh of object for fast look up
			x.second->VertexID = vertexbuffer;
			//Inster to list of buffers
			vertexBuffers.insert({ x.first, vertexbuffer });
			

			//Generate VBO indexes
			GLuint indexbuffer;
			glGenBuffers(1, &indexbuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, x.second->mesh->Indices.size() * sizeof(unsigned int),
				x.second->mesh->Indices.data(), GL_STATIC_DRAW);
			//Store in mesh of object for fast look up
			//Inster to list of buffers
			indexBuffers.insert({ x.first, indexbuffer });
		}

		oldData = data;
	}

	void VertexBufferManger::updateVetexBuffer(uint32_t entityID)
	{
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

		Mesh* m = (Mesh*)ComponentMesh::getInstance()->getData(entityID);

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