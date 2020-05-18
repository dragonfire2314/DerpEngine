#pragma once

#include "../../core/ecs/components/ComponentMesh.h"
#include "../../core/ecs/components/ComponentStructs.h"

#include <unordered_map>
#include <vector>

#include <GL/glew.h>

namespace DERP {

	class VertexBufferManger
	{
	private:
		//Key - entityID
		//Value - vertexBufferID
		std::unordered_map<uint32_t, GLuint> vertexBuffers;
	public:
		void makeVertexBuffers();
		GLuint getVertexBuffer(uint32_t entityID);
	};

}

