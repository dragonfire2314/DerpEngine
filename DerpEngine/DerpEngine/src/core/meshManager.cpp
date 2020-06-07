#include "MeshManager.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/material.h>

#include <assert.h>

#include <iostream>

#include <GL/glew.h>
#include "../../external/glm/glm.hpp"


#include "Animation/AnimationDataTypes.h"
#include "TextureManager.h"

namespace DERP {

	//Helper Function
	inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
	{
		glm::mat4 to;

		to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
		to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
		to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
		to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

		return to;
	}

	inline glm::vec3 aiVector3dToGlm(const aiVector3D* from)
	{
		glm::vec3 to;

		to.x = from->x;
		to.y = from->y;
		to.z = from->z;

		return to;
	}

	inline glm::quat aiQuaternionToGlm(const aiQuaternion* from)
	{
		glm::quat to;

		to.x = from->x;
		to.y = from->y;
		to.z = from->z;
		to.w = from->w;

		return to;
	}

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
			objl::Mesh* ms = new objl::Mesh();

			//Generate aiMesh
			aiMesh* mesh = scene->mMeshes[i];

			//Create Verts
			std::vector<objl::Vertex> verts;
			verts.reserve(mesh->mNumVertices);

			//Create bones
			std::vector<Bone> bones;
			std::unordered_map<std::string, uint32_t> boneMap;

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

			if (mesh->HasBones())
			{
				// load bones
				for (uint32_t i = 0; i < mesh->mNumBones; i++)
				{
					uint32_t bone_index = 0;
					std::string bone_name(mesh->mBones[i]->mName.data);

					if (ms->boneMapping.find(bone_name) == ms->boneMapping.end())
					{
						// Allocate an index for a new bone
						bone_index = ms->numberOfBones;
						ms->numberOfBones++;
						objl::BoneMatrix bi;
						ms->boneMatrices.push_back(bi);
						ms->boneMatrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix;
						ms->boneMapping[bone_name] = bone_index;
					}
					else
					{
						bone_index = ms->boneMapping[bone_name];
					}

					for (uint32_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
					{
						uint32_t vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
						float weight = mesh->mBones[i]->mWeights[j].mWeight;

						for (uint32_t i = 0; i < 4; i++)
						{
							if (verts[vertex_id].BoneWeights[i] == 0.0)
							{
								verts[vertex_id].BoneID[i] = bone_index;
								verts[vertex_id].BoneWeights[i] = weight;
								break;
							}
						}
					}
				}
				//bones.reserve(mesh->mNumBones);
				//for (int bi = 0; bi < mesh->mNumBones; bi++)
				//{
				//	aiBone* bone = mesh->mBones[bi];
				//	Bone d_bone;
				//	d_bone.offset = aiMatrix4x4ToGlm(&bone->mOffsetMatrix);

				//	bones.push_back(d_bone);

				//	boneMap.insert({ std::string(bone->mName.C_Str()), bi });

				//	//Add bone to verts
				//	for (int vi = 0; vi < bone->mNumWeights; vi++)
				//	{
				//		unsigned int vID = bone->mWeights[vi].mVertexId;
				//		float weight = bone->mWeights[vi].mWeight;

				//		if (verts[vID].BoneWeights.x == 0) {
				//			verts[vID].BoneID.x = bi;
				//			verts[vID].BoneWeights.x = weight;
				//		}
				//		else if (verts[vID].BoneWeights.y == 0) {
				//			verts[vID].BoneID.y = bi;
				//			verts[vID].BoneWeights.y = weight;
				//		}
				//		else if (verts[vID].BoneWeights.z == 0) {
				//			verts[vID].BoneID.z = bi;
				//			verts[vID].BoneWeights.z = weight;
				//		}
				//		else if (verts[vID].BoneWeights.w == 0) {
				//			verts[vID].BoneID.w = bi;BN
				//			verts[vID].BoneWeights.w = weight;
				//		}
				//		else
				//			printf("ERROR: Out of bone ID spots");
				//	}
				//}
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
			
			ms->Vertices = verts;
			ms->Indices = ind;
			//ms->bones = bones;
			//ms->boneNameToID = boneMap;

			//Give the mesh a texture
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString path;

			aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &path);
			//aiGetMaterialColor(material, );

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
			ms->MeshMaterial.isDiffuseTexture = true;

			//std::reverse(fName.begin(), fName.end());
			ms->MeshName = filePath;

			//Add to impMeshes
			impMeshes.push_back(ms);
		}

		newMeshes.insert({ counter, impMeshes });

		printf("Model Counter: %i\n", counter);
		
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