#include "Components.h"

#include <iostream>

namespace DERP
{
	//All components are stored in this file
	void Transform::print() 
	{
		std::cout << "Transform: " << std::endl;
		std::cout << "	Position: " << position.x << " " << position.y << " " << position.z << std::endl;
		std::cout << "	Rotation: " << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
		std::cout << "	Sacle   : " << scale.x << " " << scale.y << " " << scale.z << std::endl;
		std::cout << "End" << std::endl;
	}



	//Mesh
	Mesh::Mesh(objl::Mesh* _mesh)
	{
		mesh = _mesh;
	}

	Mesh::Mesh()
	{
		/*objl::Vertex v = {
			{-1.0f, -1.0f, 0.0f }, { 0,0,0 }, { 0,0 }
		};
		objl::Vertex v2 = {
			{1.0f, -1.0f, 0.0f}, { 0,0,0 }, { 0,0 }
		};
		objl::Vertex v3 = {
			{0.0f, 1.0f, 0.0f}, { 0,0,0 }, { 0,0 }
		};

		objl::Mesh m;
		m.Vertices.push_back(v);
		m.Vertices.push_back(v2);
		m.Vertices.push_back(v3);

		m.Indices.push_back(0);
		m.Indices.push_back(1);
		m.Indices.push_back(2);*/
	}

	//Material
	void Material::setShader(std::string _vertexShader, std::string _pixleShader)
	{
		vertexShader = _vertexShader;
		pixelShader = _pixleShader;
	}
}