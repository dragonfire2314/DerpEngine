#include "TextureManagerGL.h"

#include "../../core/TextureManager.h"
#include "../../core/ecs/components/ComponentMaterial.h"

namespace DERP {

	void TextureManagerGL::LoadTextures()
	{
		std::unordered_map<uint32_t, Material*> data = ComponentMaterial::getInstance()->getDataMap();

		for (auto x : data) 
		{
			//Check if a texture exists
			if (x.second->mat == nullptr) continue;
			TextureData texData;

			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			TextureManager textureManager;
			texData = textureManager.loadTexture(x.second->mat->map_Kd_ID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texData.width, texData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.data);
			//glGenerateMipmap(GL_TEXTURE_2D);

			GLTextureIDs.insert({ x.first, texture });
		}
	}

	GLuint TextureManagerGL::updateTexture(uint32_t entityID)
	{
		//Check for vertex buffer
		if (GLTextureIDs.find(entityID) == GLTextureIDs.end()) {
			//Create a new one
			GLTextureIDs.insert({ entityID, 0 }); //Temp data here, will be replaced at end of function
		}
		else {
			//Destory the current
			glDeleteTextures(1, &(GLTextureIDs[entityID]));
		}

		TextureData texData;

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Material* m = (Material*)ComponentMaterial::getInstance()->getData(entityID);

		TextureManager textureManager;
		texData = textureManager.loadTexture(m->mat->map_Kd_ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texData.width, texData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		//GLTextureIDs.insert({ x.first, texture });
		GLTextureIDs[entityID] = texture;
		return 0;
	}

	GLuint TextureManagerGL::getTexture(uint32_t entityID)
	{
		if (GLTextureIDs.find(entityID) == GLTextureIDs.end()) {
			return NULL;
		}
		else {
			return GLTextureIDs[entityID];
		}
	}

}