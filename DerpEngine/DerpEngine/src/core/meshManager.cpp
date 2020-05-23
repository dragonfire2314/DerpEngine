#include "MeshManager.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/material.h>

#include <assert.h>

#include <iostream>

#include "TextureManager.h"

namespace DERP {

	MeshManager::MeshManager() 
	{

	}

	MeshManager::~MeshManager() 
	{

	}

	int MeshManager::loadMeshes(std::string filePath)
	{
		const aiScene* scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		if (!scene) {
			std::cerr << "Could not load file " << filePath << aiGetErrorString() << std::endl;
			return -1;
		}

		std::vector<objl::Mesh*> impMeshes;
		impMeshes.reserve(scene->mNumMeshes);

		TextureManager textureManager;

		for (int i = 0; i < scene->mNumMeshes; i++) 
		{
			//Generate aiMesh
			aiMesh* mesh = scene->mMeshes[i];

			//Create Verts
			std::vector<objl::Vertex> verts;
			verts.reserve(mesh->mNumVertices);

			//Add verts in the mesh
			for (int vi = 0; vi < mesh->mNumVertices; vi++) 
			{
				objl::Vertex v;

				v.Position.X = mesh->mVertices[vi].x;
				v.Position.Y = mesh->mVertices[vi].y;
				v.Position.Z = mesh->mVertices[vi].z;

				v.Normal.X = mesh->mNormals[vi].x;
				v.Normal.Y = mesh->mNormals[vi].y;
				v.Normal.Z = mesh->mNormals[vi].z;

				v.TextureCoordinate.X = mesh->mTextureCoords[0][vi].x;
				v.TextureCoordinate.Y = mesh->mTextureCoords[0][vi].y;

				//Add to verts
				verts.push_back(v);
			}

			//Indices
			std::vector<unsigned int> ind;
			ind.reserve((size_t)mesh->mNumFaces * 3);

			for (int vi = 0; vi < mesh->mNumFaces; vi++)
			{
				assert(mesh->mFaces[vi].mNumIndices == 3);

				ind.push_back(mesh->mFaces[vi].mIndices[0]);
				ind.push_back(mesh->mFaces[vi].mIndices[1]);
				ind.push_back(mesh->mFaces[vi].mIndices[2]);
			}

			//Create this mesh
			objl::Mesh* ms = new objl::Mesh(verts, ind);

			//Give the mesh a texture
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString path;

			aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &path);

			//Build file path stuff
			std::string temp;
			std::string fName;
			temp.reserve(filePath.size() + path.length);
			bool nameEnd = false;
			for (int i = filePath.size(); i > 0; i--) {
				if (filePath[i] == '/') {
					temp = filePath;
					temp.append(path.C_Str());
					break;
				}
				fName.push_back(filePath[i]);
				filePath.erase(i, 1);
			}

			//Save diffuse map
			ms->MeshMaterial.map_Kd = temp;
			ms->MeshMaterial.map_Kd_ID = textureManager.addTexture(temp);

			std::reverse(fName.begin(), fName.end());
			ms->MeshName = fName;

			//Add to impMeshes
			impMeshes.push_back(ms);
		}

		newMeshes.insert({ counter, impMeshes });
		
		counter++;

		return counter - 1;
	}

	objl::Material* MeshManager::getMaterial(uint32_t fileID, uint32_t meshID)
	{
		return &(getMesh(fileID, meshID)->MeshMaterial);
	}

	objl::Mesh* MeshManager::getMesh(uint32_t fileID, uint32_t meshID)
	{
		if (newMeshes.find(fileID) == newMeshes.end()) {
			return nullptr;
		}
		else {
			return (newMeshes[fileID][meshID]);
		}
	}

	std::vector<objl::Mesh*>* MeshManager::getMeshes(uint32_t fileID)
	{
		if (newMeshes.find(fileID) == newMeshes.end()) {
			return nullptr;
		}
		else {
			return &(newMeshes[fileID]);
		}
	}

	std::unordered_map<uint32_t, std::vector<objl::Mesh*>> MeshManager::getAllData()
	{
		return newMeshes;
	}

	//Debug
	void MeshManager::debug_print_mesh(uint32_t key) 
	{
		/*for (int i = 0; i < meshes[key]->LoadedMeshes.size(); i++) 
		{
			objl::Mesh curMesh = meshes[key]->LoadedMeshes[i];

			printf("%i :Mesh name: %s", i, curMesh.MeshName.c_str());
		}*/
	}
}