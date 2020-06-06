#pragma once

//#include <assimp/scene.h>

#include <string>

namespace DERP 
{
	int loadAnimationFromFile(std::string path);

	void getAnim(int animID);

	void updateAnimations();
}