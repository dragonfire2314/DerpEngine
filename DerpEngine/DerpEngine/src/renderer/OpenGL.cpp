#include "OpenGL.h"

#include <unordered_map>
#include <GL/glew.h>

namespace DERP {

	void OpenGL::Render()
	{
		//Must render all mesh components
		std::unordered_map<uint32_t, Mesh*> data = ComponentMesh::getInstance()->getDataMap();

		//Loop though meshes
		for (auto x : data) 
		{
			//Render
			GLuint VertexArrayID;
			glGenVertexArrays(1, &VertexArrayID);
			glBindVertexArray(VertexArrayID);
		}
	}

	void OpenGL::ClearScreen()
	{

	}

}