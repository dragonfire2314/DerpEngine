#pragma once

#include "ecs/components/ComponentStructs.h"

#include <unordered_map>

namespace DERP {

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