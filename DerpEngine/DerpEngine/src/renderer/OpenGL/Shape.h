#pragma once

#include <unordered_map>

#include "../../../external/glm/glm.hpp"

namespace DERP 
{
	namespace SHAPE 
	{
		enum Shapes { POINT };

		struct ShapesData {
			//Position
			glm::vec3 position = glm::vec3(0, 0, 0);
			//Shape
			Shapes type = POINT;
		};

		void executeDraw(glm::mat4 View, glm::mat4 Projection);

		//Will only draw for one frame
		void addShape(glm::vec3 position, Shapes type);

		void initShapes();
	}
}