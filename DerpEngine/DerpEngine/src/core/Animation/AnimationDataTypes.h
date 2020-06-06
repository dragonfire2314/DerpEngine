#pragma once

#include <string>
#include <vector>

#include "../../../external/glm/glm.hpp"
#include "../../../external/glm/gtx/quaternion.hpp"

namespace DERP 
{
	struct Bone 
	{
		glm::mat4 offset = glm::mat4(1.0f);
	};

	struct NodeAnim 
	{
		std::string name;

		glm::mat4 transformMat = glm::mat4(1.0f);
		NodeAnim* parent;
		std::vector<NodeAnim*> Children;

		glm::vec3 *position;
		glm::quat *rotation;
		glm::vec3 *scale;
	};

	struct Animation 
	{
		double duration;
		double ticksPerSecond;
		//NodeAnim* channels;
		NodeAnim* root;
	};
}