#pragma once

//#include <assimp/scene.h>

#include <string>

namespace DERP 
{
	struct Animation_Info 
	{
		uint32_t animationCount = 0;
		//Key used to acess animations
		uint32_t animationGroupID;
		//Array of animation names that were imported
		std::string* animationNames;
	};

	Animation_Info loadAnimationFromFile(std::string path);

	void updateAnimations(uint32_t shader_program, uint32_t entityID);

	uint32_t getAnimationClipID(uint32_t groupID, std::string name);


	//int loadAnimationFromFile(std::string path);

	//void getAnim(int animID);

	//void updateAnimations();
}