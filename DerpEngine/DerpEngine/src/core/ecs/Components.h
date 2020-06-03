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

	struct Force 
	{
		glm::vec3 force = glm::vec3(0, 0, 0);
	};

	struct DERP_API RigidBody 
	{
		//Settings
		bool useGravity = false;
		bool useComplexPhysics = false;

		//Simple partical physics
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);

		float gravityAmount = -5;

		float inverseMass = 0;
		float dampaning = 0.95;

		std::vector<Force> forces;

		//Complex rigidbody rotation physics
		glm::vec4 angularVelocity;
	};

	enum ColliderType { NONE, BOX, SPHERE };
	struct DERP_API Collider 
	{
		ColliderType type = NONE;
		bool isTrigger = false;
	};

	struct DERP_API BoxCollider : public Collider
	{
		BoxCollider();
		glm::vec3 halfSides = glm::vec3(0.5, 0.5, 0.5);
	};

	struct DERP_API SphereCollider : public Collider
	{
		SphereCollider();
		float radius = 1;
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