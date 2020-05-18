#include "VertexBufferManger.h"

namespace DERP {

	void VertexBufferManger::makeVertexBuffers()
	{
		//Get mesh list
		std::unordered_map<uint32_t, Mesh*> data = ComponentMesh::getInstance()->getDataMap();

		//Loop though meshes
		for (auto x : data) 
		{
			//Generate VBO for each mesh
			GLuint vertexbuffer;
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, x.second->mesh->Vertices.size(), x.second->mesh->Vertices.data(), GL_STATIC_DRAW);
			//Store in mesh of object for fast look up
			x.second->VertexID = vertexbuffer;
			//Inster to list of buffers
			vertexBuffers.insert({ x.first, vertexbuffer });
		}
	}

}