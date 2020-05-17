#pragma once

#include "../../core.h"

#include "../../../../external/glm/glm.hpp"
#include "../../../../external/glm/gtx/quaternion.hpp"
#include "../../../../external/objLoader/OBJ_Loader.h"

#include "../Component.h"

namespace DERP {

	class DERP_API Transform
	{
	public:
		glm::vec3 position;
		glm::quat rotation;
	};

	class DERP_API Mesh 
	{
	public:
		Mesh(objl::Mesh* _mesh);
		Mesh();

		objl::Mesh* mesh;
	};

}
//
//struct Mesh 
//{
//	size_t* EntityID;
//};
//
//struct Material 
//{
//	glm::vec4 color;
//};
//
//struct Rigidbody 
//{
//	float velocity;
//};
//
//struct Script 
//{
//	
//};