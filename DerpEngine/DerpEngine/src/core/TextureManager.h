#pragma once

#include <unordered_map>

namespace DERP {

	//Texture Data structure
	struct TextureData
	{
		unsigned char* data;
		int width;
		int height;
	};

	/*
	
	TODO
		-Key reuseal
	*/

	class TextureManager 
	{
	private:
		//Key - TextureID
		//Value - filePath
		

		static uint32_t counter;
	public:
		struct this_FileInfo {
			std::string path;
			std::string name;
		};
		static std::unordered_map<uint32_t, this_FileInfo> texturePaths;

		TextureData loadTexture(uint32_t textureID);
		int addTexture(std::string filePath);
		void freeTexture(unsigned char* data);
	};

}