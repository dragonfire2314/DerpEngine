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