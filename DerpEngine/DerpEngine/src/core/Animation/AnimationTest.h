#pragma once

#include "../ecs/Components.h"

#include <string>

namespace DERP 
{
	void initAnimTest(uint32_t shader_program);
	void drawAnim(uint32_t shaders_program);
	objl::Mesh* loadAnimationAndMeshFromFile(std::string path);
}