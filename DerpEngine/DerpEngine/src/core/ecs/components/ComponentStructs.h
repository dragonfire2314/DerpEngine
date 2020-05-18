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
		//glm::vec3 rotation;
		glm::vec3 scale;
	};

	class DERP_API Mesh 
	{
	public:
		uint32_t VertexID;

		Mesh(objl::Mesh* _mesh);
		Mesh();

		objl::Mesh* mesh;
	};

	class DERP_API Material 
	{
	public:
		uint32_t ShaderID;

		//Shader
		std::string vertexShader = "Teal";
		std::string pixelShader;

		//Material
		objl::Material* mat;

		//Functions
		void setShader(std::string _vertexShader, std::string _pixleShader);
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