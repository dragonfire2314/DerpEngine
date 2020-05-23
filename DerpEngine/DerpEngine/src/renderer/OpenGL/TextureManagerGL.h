#pragma once

#include <unordered_map>
#include <GL/glew.h>

namespace DERP {

	class TextureManagerGL
	{
	private:
		//Key - entityID
		//Value - opengl textureID
		std::unordered_map<uint32_t, GLuint> GLTextureIDs;
	public:
		void LoadTextures();
		GLuint getTexture(uint32_t entityID);

		GLuint updateTexture(uint32_t entityID);
	};

}