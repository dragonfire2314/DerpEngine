#pragma once
#include <vector>

namespace DERP {

	class System
	{
	public:
		std::vector<uint32_t> Entities;
	};

	class Sys_Renderer : public System {};
	class Sys_Shader : public System {};
	class Sys_Vertex : public System {};
	class Sys_Scripts : public System {};
	class Sys_Cameras : public System {};
	class Sys_DirLight : public System {};
	class Sys_PointLight : public System {};
	class Sys_Physics : public System {};
	class Sys_BoxCollider : public System {};
	class Sys_SphereCollider : public System {};

	extern Sys_Renderer* sys_renderer;
	extern Sys_Shader* sys_shader;
	extern Sys_Vertex* sys_vertex;
	extern Sys_Scripts* sys_scripts;
	extern Sys_Cameras* sys_cameras;
	extern Sys_DirLight* sys_dirLight;
	extern Sys_PointLight* sys_pointLight;
	extern Sys_Physics* sys_physics;
	extern Sys_BoxCollider* sys_boxCollider;
	extern Sys_SphereCollider* sys_sphereCollider;
}