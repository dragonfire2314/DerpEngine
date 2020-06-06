#include "AnimationTest.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/material.h>

#include <assert.h>

#include <iostream>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include "../../../external/glm/glm.hpp"

namespace DERP
{
	struct BoneMatrix
	{
		aiMatrix4x4 offset_matrix;
		aiMatrix4x4 final_world_transform;
	};

	std::unordered_map<std::string, uint32_t> m_bone_mapping; // maps a bone name and their index
	uint32_t m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;
	aiMatrix4x4 m_global_inverse_transform;

	GLuint m_bone_location[100];
	float ticks_per_second = 0.0f;

	const aiScene* scene;

	inline glm::mat4 ANIM_aiMatrix4x4ToGlm(const aiMatrix4x4* from)
	{
		glm::mat4 to;

		to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
		to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
		to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
		to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

		return to;
	}

	void initAnimTest(uint32_t shader_program)
	{
		for (uint32_t i = 0; i < 100; i++) // get location all matrices of bones
		{
			std::string name = "bones[" + std::to_string(i) + "]";
			m_bone_location[i] = glGetUniformLocation(shader_program, name.c_str());
		}
	}

	objl::Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	
	objl::Mesh* loadAnimationAndMeshFromFile(std::string path)
	{
		scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene) {
			std::cerr << "Could not load file " << path << aiGetErrorString() << std::endl;
		}

		m_global_inverse_transform = scene->mRootNode->mTransformation;
		m_global_inverse_transform.Inverse();

		if (scene->mAnimations[0]->mTicksPerSecond != 0.0)
		{
			ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
		}
		else
		{
			ticks_per_second = 25.0f;
		}

		aiMesh* mesh = scene->mMeshes[0];

		return processMesh(mesh, scene);
	}

	objl::Mesh* processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::cout << "bones: " << mesh->mNumBones << " vertices: " << mesh->mNumVertices << std::endl;
		//system("PAUSE");

		std::vector<objl::Vertex> vertices;
		std::vector<uint32_t> indices;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumVertices); 

		//vertices
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			objl::Vertex vertex;
			vertex.Position.X = mesh->mVertices[i].x;
			vertex.Position.Y = mesh->mVertices[i].y;
			vertex.Position.Z = mesh->mVertices[i].z;

			if (mesh->mNormals != NULL)
			{
				vertex.Normal.X = mesh->mNormals[i].x;
				vertex.Normal.Y = mesh->mNormals[i].y;
				vertex.Normal.Z = mesh->mNormals[i].z;
			}

			if (mesh->mTextureCoords[0])
			{
				vertex.TextureCoordinate.X = mesh->mTextureCoords[0][i].x;
				vertex.TextureCoordinate.Y = mesh->mTextureCoords[0][i].y;
			}

			vertices.push_back(vertex);
		}

		//indices
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		// load bones
		for (uint32_t i = 0; i < mesh->mNumBones; i++)
		{
			uint32_t bone_index = 0;
			std::string bone_name(mesh->mBones[i]->mName.data);

			if (m_bone_mapping.find(bone_name) == m_bone_mapping.end())
			{
				// Allocate an index for a new bone
				bone_index = m_num_bones;
				m_num_bones++;
				BoneMatrix bi;
				m_bone_matrices.push_back(bi);
				m_bone_matrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix;
				m_bone_mapping[bone_name] = bone_index;
			}
			else
			{
				bone_index = m_bone_mapping[bone_name];
			}

			for (uint32_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				uint32_t vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;

				for (uint32_t i = 0; i < 4; i++)
				{
					if (vertices[vertex_id].BoneWeights[i] == 0.0)
					{
						vertices[vertex_id].BoneID[i] = bone_index;
						vertices[vertex_id].BoneWeights[i] = weight;
						break;
					}
				}
			}
		}

		//Print out all bones ids
		for (auto x : vertices) {
			//printf("Bone ID's: %i, %i, %i, %i\n", x.BoneID[0], x.BoneID[1], x.BoneID[2], x.BoneID[3]);
			//printf("Bone We's: %f, %f, %f, %f\n", x.BoneWeights[0], x.BoneWeights[1], x.BoneWeights[2], x.BoneWeights[3]);
		}

		//Build the mesh
		objl::Mesh* outMesh = new objl::Mesh(vertices, indices);
		outMesh->MeshName = "Made by anim";
		//outMesh->isAnimation = true;
		return outMesh;
	}

	const aiNodeAnim* findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name)
	{
		for (uint32_t i = 0; i < p_animation->mNumChannels; i++)
		{
			const aiNodeAnim* node_anim = p_animation->mChannels[i];
			if (std::string(node_anim->mNodeName.data) == p_node_name)
			{
				return node_anim;
			}
		}

		return nullptr;
	}

	uint32_t findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		// ����� ���� ������� ����� ����� ����� ������� ���������� ����� ������ ��������
		for (uint32_t i = 0; i < p_node_anim->mNumPositionKeys - 1; i++) // �������� ����� ��������
		{
			if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime) // �������� �� �������� ��������� !!!
			{
				return i; // �� ������� ������ �������� !!!!!!!!!!!!!!!!!! ����������������������������
			}
		}

		assert(0);
		return 0;
	}
	uint32_t findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		// ����� ���� ������� ����� ����� ����� ������� ���������� ����� ������ ��������
		for (uint32_t i = 0; i < p_node_anim->mNumRotationKeys - 1; i++) // �������� ����� ��������
		{
			if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime) // �������� �� �������� ��������� !!!
			{
				return i; // �� ������� ������ �������� !!!!!!!!!!!!!!!!!! ����������������������������
			}
		}

		assert(0);
		return 0;
	}
	uint32_t findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		// ����� ���� ������� ����� ����� ����� ������� ���������� ����� ������ ��������
		for (uint32_t i = 0; i < p_node_anim->mNumScalingKeys - 1; i++) // �������� ����� ��������
		{
			if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime) // �������� �� �������� ��������� !!!
			{
				return i; // �� ������� ������ �������� !!!!!!!!!!!!!!!!!! ����������������������������
			}
		}

		assert(0);
		return 0;
	}

	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend)
	{
		//cout << a.w + a.x + a.y + a.z << endl;
		a.Normalize();
		b.Normalize();

		aiQuaternion result;
		float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		float one_minus_blend = 1.0f - blend;

		if (dot_product < 0.0f)
		{
			result.x = a.x * one_minus_blend + blend * -b.x;
			result.y = a.y * one_minus_blend + blend * -b.y;
			result.z = a.z * one_minus_blend + blend * -b.z;
			result.w = a.w * one_minus_blend + blend * -b.w;
		}
		else
		{
			result.x = a.x * one_minus_blend + blend * b.x;
			result.y = a.y * one_minus_blend + blend * b.y;
			result.z = a.z * one_minus_blend + blend * b.z;
			result.w = a.w * one_minus_blend + blend * b.w;
		}

		return result.Normalize();
	}

	aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumPositionKeys == 1) // Keys ��� ������� �����
		{
			return p_node_anim->mPositionKeys[0].mValue;
		}

		uint32_t position_index = findPosition(p_animation_time, p_node_anim); // ������ ������ �������� ����� ������� ������
		uint32_t next_position_index = position_index + 1; // ������ ��������� �������� �����
		assert(next_position_index < p_node_anim->mNumPositionKeys);
		// ���� ����� �������
		float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);
		// ������ = (���� ������� ������ �� ������ �������� ��������� �����) / �� ���� ����� �������
		float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
		aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
		aiVector3D delta = end - start;

		return start + factor * delta;
	}
	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumRotationKeys == 1) // Keys ��� ������� �����
		{
			return p_node_anim->mRotationKeys[0].mValue;
		}

		uint32_t rotation_index = findRotation(p_animation_time, p_node_anim); // ������ ������ �������� ����� ������� ������
		uint32_t next_rotation_index = rotation_index + 1; // ������ ��������� �������� �����
		assert(next_rotation_index < p_node_anim->mNumRotationKeys);
		// ���� ����� �������
		float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
		// ������ = (���� ������� ������ �� ������ �������� ��������� �����) / �� ���� ����� �������
		float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

		//cout << "p_node_anim->mRotationKeys[rotation_index].mTime: " << p_node_anim->mRotationKeys[rotation_index].mTime << endl;
		//cout << "p_node_anim->mRotationKeys[next_rotaion_index].mTime: " << p_node_anim->mRotationKeys[next_rotation_index].mTime << endl;
		//cout << "delta_time: " << delta_time << endl;
		//cout << "animation_time: " << p_animation_time << endl;
		//cout << "animation_time - mRotationKeys[rotation_index].mTime: " << (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) << endl;
		//cout << "factor: " << factor << endl << endl << endl;

		assert(factor >= 0.0f && factor <= 1.0f);
		aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
		aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

		return nlerp(start_quat, end_quat, factor);
	}
	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumScalingKeys == 1) // Keys ��� ������� �����
		{
			return p_node_anim->mScalingKeys[0].mValue;
		}

		uint32_t scaling_index = findScaling(p_animation_time, p_node_anim); // ������ ������ �������� ����� ������� ������
		uint32_t next_scaling_index = scaling_index + 1; // ������ ��������� �������� �����
		assert(next_scaling_index < p_node_anim->mNumScalingKeys);
		// ���� ����� �������
		float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
		// ������ = (���� ������� ������ �� ������ �������� ��������� �����) / �� ���� ����� �������
		float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
		aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
		aiVector3D delta = end - start;

		return start + factor * delta;
	}

	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
	{
		std::string node_name(p_node->mName.data);

		const aiAnimation* animation = scene->mAnimations[0];
		aiMatrix4x4 node_transform = p_node->mTransformation;

		const aiNodeAnim* node_anim = findNodeAnim(animation, node_name); // ����� ������� �� ����� ����

		if (node_anim)
		{
			//scaling
			aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
			//aiVector3D scaling_vector = node_anim->mPositionKeys[0].mValue;
			aiMatrix4x4 scaling_matr;
			aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

			//rotation
			aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
			//aiQuaternion rotate_quat = node_anim->mRotationKeys[0].mValue;
			aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

			//translation
			aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);//node_anim->mPositionKeys[((uint32_t)TIME::time % 3)].mValue;
			//aiVector3D translate_vector = node_anim->mScalingKeys[0].mValue;
			aiMatrix4x4 translate_matr;
			aiMatrix4x4::Translation(translate_vector, translate_matr);

			node_transform = translate_matr * rotate_matr * scaling_matr;
		}

		aiMatrix4x4 global_transform = parent_transform * node_transform;

		if (m_bone_mapping.find(node_name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
		{
			//printf("Node Name: %s : ", node_name.c_str());
			uint32_t bone_index = m_bone_mapping[node_name];
			//printf("Node Index: %i\n", bone_index);
			m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
		}

		for (uint32_t i = 0; i < p_node->mNumChildren; i++)
		{
			readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
		}
	}

	void boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms)
	{
		aiMatrix4x4 identity_matrix; // = mat4(1.0f);

		double time_in_ticks = time_in_sec * ticks_per_second;
		float animation_time = std::fmod(time_in_ticks, scene->mAnimations[0]->mDuration);

		readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);

		transforms.resize(m_num_bones);

		for (uint32_t i = 0; i < m_num_bones; i++)
		{
			transforms[i] = m_bone_matrices[i].final_world_transform;
		}
	}

	void drawAnim(uint32_t shaders_program)
	{
		std::vector<aiMatrix4x4> transforms;
		boneTransform(TIME::time, transforms);

		//transforms.resize(m_num_bones);

		const int t = 1;
		//printf("a: %f, %f, %f, %f\n", transforms[t].a1, transforms[t].a2, transforms[t].a3, transforms[t].a4);
		//printf("b: %f, %f, %f, %f\n", transforms[t].b1, transforms[t].b2, transforms[t].b3, transforms[t].b4);
		//printf("c: %f, %f, %f, %f\n", transforms[t].c1, transforms[t].c2, transforms[t].c3, transforms[t].c4);
		//printf("d: %f, %f, %f, %f\n", transforms[t].d1, transforms[t].d2, transforms[t].d3, transforms[t].d4);

		for (uint32_t i = 0; i < transforms.size(); i++) // move all matrices for actual model position to shader
		{
			glUniformMatrix4fv(m_bone_location[i], 1, GL_FALSE, &ANIM_aiMatrix4x4ToGlm(&transforms[i])[0][0]);
		}
	}
}
