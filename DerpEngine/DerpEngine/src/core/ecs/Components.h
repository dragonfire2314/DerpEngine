#pragma once

#include "../core.h"

#include "../../../external/glm/glm.hpp"
#include "../../../external/glm/gtx/quaternion.hpp"
#include "../../../external/objLoader/OBJ_Loader.h"
#include "Scriptable.h"

#include <unordered_map>

namespace DERP 
{
	//All components are stored in this file
	struct DERP_API Transform 
	{
		void print();

		glm::vec3 forward();

		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::quat rotation;
		glm::vec3 scale = glm::vec3(0, 0, 0);
	};

	struct DERP_API Script
	{
		Scriptable* script;
	};

	struct DERP_API RigidBody 
	{
		float velocity;
		glm::vec3 dir = glm::vec3(0, 0, 0);
	};

	struct DERP_API Mesh 
	{
		uint32_t VertexID;

		Mesh(objl::Mesh* _mesh);
		Mesh();

		objl::Mesh* mesh = nullptr;
	};

	struct DERP_API Material
	{
		Material();

		uint32_t ShaderID;

		//Shader
		std::string vertexShader = "defaultVertexShader.glsl";
		std::string pixelShader = "defaultPixelShader.glsl";

		//Material
		objl::Material* mat;

		//Unitform List


		//Material Options
		bool caseShadows = false;
		bool reciveShadows = false;

		//Functions
		void setShader(std::string _vertexShader, std::string _pixleShader);
	};

	struct DERP_API Camera
	{
		//Obsolete
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 frount = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	};

	struct DERP_API PointLight 
	{
		float range;
	};

	struct DERP_API DirectionalLight 
	{
		glm::vec3 direction;
	};
}