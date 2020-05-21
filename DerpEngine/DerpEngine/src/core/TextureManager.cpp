#include "TextureManager.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb/stb_image.h"

namespace DERP {

	std::unordered_map<uint32_t, TextureManager::this_FileInfo> TextureManager::texturePaths;
	uint32_t TextureManager::counter = 0;

	TextureData TextureManager::loadTexture(uint32_t textureID)
	{
		printf("I cant beilve that this was called, it worked, nice\n");

		TextureData td;

		//Check if texture ID is valid
		if (texturePaths.find(textureID) == texturePaths.end())
		{
			std::cerr << "File ID doesn't exist " << textureID << std::endl;
			td.data = nullptr;
			return td;
		}
		else
		{
			int width, height;
			int n;
			//Load the texture
			unsigned char* data = stbi_load(texturePaths[textureID].path.c_str(), &width, &height, &n, 0);

			printf("TEST SIZE: %i\n", n);

			//Check if loaded
			if (data == nullptr) 
			{
				std::cerr << "Could not load image " << texturePaths[textureID].path << std::endl;
				td.data = nullptr;
				return td;
			}
			//Return texture
			td.data = data;
			td.height = height;
			td.width = width;
			return td;
		}
	}

	int TextureManager::addTexture(std::string filePath)
	{
		this_FileInfo fileInfo;
		fileInfo.path = filePath;

		//Build file name
		for (int i = filePath.size(); i > 0; i--) {
			if (filePath[i] == '/') {
				break;
			}
			fileInfo.name.push_back(filePath[i]);
		}
		std::reverse(fileInfo.name.begin(), fileInfo.name.end());

		//Check if it already exists
		for (auto x : texturePaths) 
		{
			if (x.second.path == filePath) 
			{
				return x.first;
			}
		}
		//Add file path
		texturePaths.insert({ counter, fileInfo });
		counter++;
		return counter - 1;
	}

	void TextureManager::freeTexture(unsigned char* data)
	{
		stbi_image_free(data);
	}

}