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

		objl::Mesh* mesh = nullptr;
	};

	class DERP_API Material 
	{
	public:
		uint32_t ShaderID;

		//Shader
		std::string vertexShader = "defaultVertexShader.glsl";
		std::string pixelShader = "defaultPixelShader.glsl";

		//Material
		objl::Material* mat;

		//Functions
		void setShader(std::string _vertexShader, std::string _pixleShader);
	};

	//Texture Data structure
	struct TextureData 
	{
		unsigned char* data;
		int width;
		int height;
	};

	class Camera 
	{
	public:
		//Obsolete
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 frount = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
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