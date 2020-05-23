#pragma once

#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "../../core/ecs/Components.h"

namespace DERP {

	class VertexBufferManger
	{
	private:
		//Key - entityID
		//Value - vertexBufferID
		std::unordered_map<uint32_t, GLuint> vertexBuffers;
		//Key - entityID
		//Value - indexBufferID
		std::unordered_map<uint32_t, GLuint> indexBuffers;
	public:
		void makeVertexBuffers();
		GLuint getVertexBuffer(uint32_t entityID);
		GLuint getIndexBuffer(uint32_t entityID);

		void updateVetexBuffer(uint32_t entityID);
	};

}

