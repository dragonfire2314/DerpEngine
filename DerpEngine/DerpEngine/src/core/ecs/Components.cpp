#include "Components.h"

#include <iostream>

namespace DERP
{
	objl::Material defualtMat;

	Material::Material() 
	{
		mat = new objl::Material();
	}

	//All components are stored in this file
	void Transform::print() 
	{
		std::cout << "Transform: " << std::endl;
		std::cout << "	Position: " << position.x << " " << position.y << " " << position.z << std::endl;
		std::cout << "	Rotation: " << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
		std::cout << "	Sacle   : " << scale.x << " " << scale.y << " " << scale.z << std::endl;
		std::cout << "End" << std::endl;
	}

	glm::vec3 Transform::forward() { return rotation * glm::vec3(0, 0, -1); }

	glm::vec3 Transform::up() { return rotation* glm::vec3(0, 1, 0); }

	glm::vec3 Transform::right() { return rotation * glm::vec3(1, 0, 0); }

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

	BoxCollider::BoxCollider()
	{
		type = BOX;
	}

	SphereCollider::SphereCollider()
	{
		type = SPHERE;
	}

	void AudioSource::play()
	{
		isPlaying = true;

		updateOccured = true;
	}

	void AudioSource::stop()
	{
		isPlaying = false;

		updateOccured = true;
	}

	void AudioSource::changeAudio(std::string _filePath)
	{
		filePath = _filePath;

		updateOccured = true;
		isPlaying = false;
	}

	std::string AudioSource::getFile() 
	{
		return filePath;
	}

	bool AudioSource::getUpdateStatus() 
	{
		return updateOccured;
	}

	void AudioSource::UpdateHandled()
	{
		updateOccured = false;
	}

	bool AudioSource::getPlayingStatus() { return isPlaying; }
	float AudioSource::getVolume() { return volume; }
	bool AudioSource::get3DStatus() { return is3D; }
	bool AudioSource::getLoopStatus() { return Loop; }
}