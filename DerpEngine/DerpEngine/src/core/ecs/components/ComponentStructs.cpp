#include "ComponentStructs.h"

#include "Transform.h"

namespace DERP {

	//Mesh
	Mesh::Mesh(objl::Mesh* _mesh)
	{
		mesh = _mesh;
	}

	Mesh::Mesh() 
	{

	}

	//Material
	void Material::setShader(std::string _vertexShader, std::string _pixleShader)
	{
		vertexShader = _vertexShader;
		pixelShader = _pixleShader;
	}
}
