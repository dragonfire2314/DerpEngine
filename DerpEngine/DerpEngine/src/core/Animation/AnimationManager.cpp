#include "AnimationManager.h"

#include "AnimationDataTypes.h"

#include "../ecs/systems/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/Components.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/material.h>

#include <gl/glew.h>

#include <iostream>
#include <unordered_map>

namespace DERP
{
	struct Animation_Data
	{
		const aiScene* scene;
		uint32_t animationNumber;
		aiMatrix4x4 global_inverse_transform;
		float tickPerSecond;
	};

	//Key - animation group ID
	//Value - map from name to anim ID
	std::unordered_map<uint32_t, std::unordered_map<std::string, uint32_t>> animIDMap;
	
	//Key - Animation ID
	//Value - Scene pointer
	std::unordered_map<uint32_t, Animation_Data> sceneMap;

	uint32_t animGroupIdCounter = 0;
	uint32_t animationIdCounter = 0;

	inline glm::mat4 ANIMATION_MANAGER_aiMatrix4x4ToGlm(const aiMatrix4x4* from)
	{
		glm::mat4 to;

		to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
		to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
		to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
		to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

		return to;
	}

	Animation_Info loadAnimationFromFile(std::string path) 
	{
		Animation_Info info; //Return value

		const aiScene* scene = aiImportFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene) {
			std::cerr << "Could not load file " << path << aiGetErrorString() << std::endl;
		}

		info.animationNames = new std::string[scene->mNumAnimations];

		//loop thourgh and store all animations in this file
		std::unordered_map<std::string, uint32_t> this_nameToId;
		for (int i = 0; i < scene->mNumAnimations; i++)
		{
			Animation_Data anim_data;
			anim_data.scene = scene;

			anim_data.global_inverse_transform = scene->mRootNode->mTransformation;
			anim_data.global_inverse_transform.Inverse();

			if (anim_data.scene->mAnimations[0]->mTicksPerSecond != 0.0)
			{
				anim_data.tickPerSecond = anim_data.scene->mAnimations[0]->mTicksPerSecond;
			}
			else
			{
				anim_data.tickPerSecond = 25.0f;
			}

			anim_data.animationNumber = i;

			sceneMap.insert({ animationIdCounter, anim_data });
			this_nameToId.insert({ std::string(scene->mAnimations[i]->mName.C_Str()), animationIdCounter });

			animationIdCounter++;

			//Gernate return info
			info.animationNames[i] = scene->mAnimations[i]->mName.C_Str();
		}

		animIDMap.insert({ animGroupIdCounter, this_nameToId });

		info.animationCount = scene->mNumAnimations;
		info.animationGroupID = animGroupIdCounter;

		animGroupIdCounter++;

		return info;
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
		for (uint32_t i = 0; i < p_node_anim->mNumPositionKeys - 1; i++)
		{
			if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime)
			{
				return i;
			}
		}

		assert(0);
		return 0;
	}
	uint32_t findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		for (uint32_t i = 0; i < p_node_anim->mNumRotationKeys - 1; i++)
		{
			if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime)
			{
				return i;
			}
		}

		assert(0);
		return 0;
	}
	uint32_t findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
	{

		for (uint32_t i = 0; i < p_node_anim->mNumScalingKeys - 1; i++) 
		{
			if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime)
			{
				return i; 
			}
		}

		assert(0);
		return 0;
	}

	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend)
	{
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
		if (p_node_anim->mNumPositionKeys == 1)
		{
			return p_node_anim->mPositionKeys[0].mValue;
		}

		uint32_t position_index = findPosition(p_animation_time, p_node_anim);
		uint32_t next_position_index = position_index + 1; 
		assert(next_position_index < p_node_anim->mNumPositionKeys);
		float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);
		float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
		aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
		aiVector3D delta = end - start;

		return start + factor * delta;
	}
	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumRotationKeys == 1)
		{
			return p_node_anim->mRotationKeys[0].mValue;
		}

		uint32_t rotation_index = findRotation(p_animation_time, p_node_anim);
		uint32_t next_rotation_index = rotation_index + 1;
		assert(next_rotation_index < p_node_anim->mNumRotationKeys);
		
		float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
		
		float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

		assert(factor >= 0.0f && factor <= 1.0f);
		aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
		aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

		return nlerp(start_quat, end_quat, factor);
	}
	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumScalingKeys == 1)
		{
			return p_node_anim->mScalingKeys[0].mValue;
		}

		uint32_t scaling_index = findScaling(p_animation_time, p_node_anim);
		uint32_t next_scaling_index = scaling_index + 1;
		assert(next_scaling_index < p_node_anim->mNumScalingKeys);
		float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
		float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
		aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
		aiVector3D delta = end - start;

		return start + factor * delta;
	}

	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform, Animation_Data anim_data, Mesh* mesh)
	{
		std::string node_name(p_node->mName.data);

		const aiAnimation* animation = anim_data.scene->mAnimations[anim_data.animationNumber];
		aiMatrix4x4 node_transform = p_node->mTransformation;

		const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);

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

		if (mesh->mesh->boneMapping.find(node_name) != mesh->mesh->boneMapping.end()) // true if node_name exist in bone_mapping
		{
			uint32_t bone_index = mesh->mesh->boneMapping[node_name];
			mesh->mesh->boneMatrices[bone_index].final_world_transform = anim_data.global_inverse_transform * global_transform * mesh->mesh->boneMatrices[bone_index].offset_matrix;
		}

		for (uint32_t i = 0; i < p_node->mNumChildren; i++)
		{
			readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform, anim_data, mesh);
		}
	}

	void boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms, Animation_Data anim_data, Mesh* mesh)
	{
		aiMatrix4x4 identity_matrix;

		double time_in_ticks = time_in_sec * anim_data.tickPerSecond;
		float animation_time = std::fmod(time_in_ticks, anim_data.scene->mAnimations[anim_data.animationNumber]->mDuration);

		readNodeHierarchy(animation_time, anim_data.scene->mRootNode, identity_matrix, anim_data, mesh);

		transforms.resize(mesh->mesh->numberOfBones);

		for (uint32_t i = 0; i < mesh->mesh->numberOfBones; i++)
		{
			transforms[i] = mesh->mesh->boneMatrices[i].final_world_transform;
		}
	}

	void initAnimation(uint32_t shader_program, Animator* a)
	{
		for (uint32_t i = 0; i < 100; i++) // get location all matrices of bones
		{
			std::string name = "bones[" + std::to_string(i) + "]";
			a->boneLocations[i] = glGetUniformLocation(shader_program, name.c_str());
		}
	}

	void updateAnimations(uint32_t shader_program, uint32_t entityID)
	{
		for (auto x : sys_animator->Entities) {
			if (entityID == x) {
				Animator* a = CM::GetComponent<Animator>(x);
				Mesh* m = CM::GetComponent<Mesh>(x);

				initAnimation(shader_program, a);

				std::vector<aiMatrix4x4> transforms;
				boneTransform(TIME::time, transforms, sceneMap[a->clipID], m);

				for (uint32_t i = 0; i < transforms.size(); i++) // move all matrices for actual model position to shader
				{
					glUniformMatrix4fv(a->boneLocations[i], 1, GL_FALSE, &ANIMATION_MANAGER_aiMatrix4x4ToGlm(&transforms[i])[0][0]);
				}
			}
		}
	}

	uint32_t getAnimationClipID(uint32_t groupID, std::string name) 
	{
		if ((animIDMap[groupID]).find(name) == (animIDMap[groupID]).end()) {
			printf("Could not find animation\n");
		}
		else {
			return (animIDMap[groupID])[name];
		}
	}







































	////Key animID;
	////Value bone vector
	//std::unordered_map<uint32_t, std::vector<Bone>> animBoneMap;
	////Key animID;
	////Value animation
	//std::unordered_map<uint32_t, Animation> animIDAnimationMap;

	///*
	//	TEMP
	//*/
	////Key animID;
	////Value bone vector
	//std::unordered_map<uint32_t, std::vector<aiBone*>> AI_animToBoneMap;
	////Key animID;
	////Value animation
	//std::unordered_map<uint32_t, aiAnimation*> AI_animToAnimationMap;
	////Key animID;
	////Value root node
	//std::unordered_map<uint32_t, aiNode*> AI_animToNodeMap;
	///*
	//	END TEMP
	//*/


	//uint32_t animCounter = 0;

	////Helper Function
	//glm::mat4 ANIM_aiMatrix4x4ToGlm(const aiMatrix4x4* from)
	//{
	//	glm::mat4 to;

	//	to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
	//	to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
	//	to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
	//	to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

	//	return to;
	//}

	//glm::vec3 ANIM_aiVector3dToGlm(const aiVector3D* from)
	//{
	//	glm::vec3 to;

	//	to.x = from->x; 
	//	to.y = from->y;
	//	to.z = from->z;

	//	return to;
	//}

	//glm::quat ANIM_aiQuaternionToGlm(const aiQuaternion* from)
	//{
	//	glm::quat to;

	//	to.x = from->x;
	//	to.y = from->y;
	//	to.z = from->z;
	//	to.w = from->w;

	//	return to;
	//}

	//NodeAnim* buildTree(aiNode* node, NodeAnim* parent)
	//{
	//	//Create a node and set its parent and the parents children
	//	NodeAnim* nodeAnim = new NodeAnim();
	//	nodeAnim->parent = parent;
	//	//Copy data from node
	//	nodeAnim->name = node->mName.C_Str();
	//	nodeAnim->transformMat = ANIM_aiMatrix4x4ToGlm(&node->mTransformation);

	//	for (int y = 0; y < node->mNumChildren; y++) {
	//		nodeAnim->Children.push_back(buildTree(node->mChildren[y], nodeAnim));
	//	}

	//	return nodeAnim;
	//}

	//void printAiNode(aiNode* node)
	//{
	//	printf("aiNode: %s\n", node->mName.C_Str());

	//	for (int y = 0; y < node->mNumChildren; y++)
	//		printAiNode(node->mChildren[y]);
	//}

	//void printNodeAnim(NodeAnim* node)
	//{
	//	printf("NodeAn: %s\n", node->name.c_str());

	//	for (int y = 0; y < node->Children.size(); y++)
	//		printNodeAnim(node->Children[y]);
	//}

	//void loadMovementData(NodeAnim* node, aiAnimation* ai)
	//{
	//	for (int ia = 0; ia < ai->mNumChannels; ia++) {
	//		if (node->name == ai->mChannels[ia]->mNodeName.C_Str()) {
	//			aiNodeAnim* aiNA = ai->mChannels[ia];
	//			//Position
	//			node->position = new glm::vec3[aiNA->mNumPositionKeys];
	//			for (int ip = 0; ip < aiNA->mNumPositionKeys; ip++) {
	//				node->position[ip] = ANIM_aiVector3dToGlm(&aiNA->mPositionKeys[0].mValue);
	//			}
	//			//Rotation
	//			node->rotation = new glm::quat[aiNA->mNumRotationKeys];
	//			for (int ir = 0; ir < aiNA->mNumRotationKeys; ir++) {
	//				node->rotation[ir] = ANIM_aiQuaternionToGlm(&aiNA->mRotationKeys[0].mValue);
	//			}
	//			//Scale
	//			node->scale = new glm::vec3[aiNA->mNumScalingKeys];
	//			for (int is = 0; is < aiNA->mNumScalingKeys; is++) {
	//				node->scale[is] = ANIM_aiVector3dToGlm(&aiNA->mScalingKeys[0].mValue);
	//			}
	//		}
	//	}

	//	for (int y = 0; y < node->Children.size(); y++)
	//		loadMovementData(node->Children[y], ai);
	//}






	//aiNodeAnim* findNodeAnim(aiAnimation* anim, std::string value) 
	//{
	//	for (int i = 0; i < anim->mNumChannels; i++) 
	//	{
	//		aiNodeAnim* node = anim->mChannels[i];
	//		if (node->mNodeName.C_Str() == value)
	//			return node;
	//	}
	//}

	//void BoneUpdater(aiNode* node, uint32_t x, aiAnimation* anim)
	//{
	//	//Do stuff with node
	//	Mesh* mesh = CM::GetComponent<Mesh>(x);
	//	uint32_t boneID = mesh->mesh->boneNameToID[std::string(node->mName.C_Str())];

	//	if (node->mParent == nullptr) {

	//	}
	//	else {
	//		mesh->mesh->bones[boneID].offset = ANIM_aiMatrix4x4ToGlm(&node->mParent->mTransformation) * ANIM_aiMatrix4x4ToGlm(&node->mTransformation);
	//	}

	//	//Go to children nodes
	//	for (int y = 0; y < node->mNumChildren; y++)
	//		BoneUpdater(node->mChildren[y], x, anim);
	//}

	//void updateAnimations() 
	//{
	//	for (auto x : sys_animator->Entities) 
	//	{
	//		aiAnimation* anim = AI_animToAnimationMap[0];

	//		BoneUpdater(AI_animToNodeMap[0], x, anim);
	//	}
	//}

	//int loadAnimationFromFile(std::string path)
	//{
	//	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	//	if (!scene) {
	//		std::cerr << "Could not load file " << path << aiGetErrorString() << std::endl;
	//		return -1;
	//	}
	//	/*
	//		SLOW CODE
	//	*/
	//	aiAnimation* aiAnim = scene->mAnimations[0];
	//	aiNode* node = scene->mRootNode;

	//	AI_animToAnimationMap.insert({ animCounter, aiAnim });
	//	AI_animToNodeMap.insert({ animCounter, node });
	//	animCounter++;

	//	/*
	//		PROBABLY BETTER CODE, ONLY TRY ONCE SLOW CODE WORKS
	//	*/
	//	//for (int i = 0; i < scene->mNumMeshes; i++)
	//	//{
	//	//	//Generate aiMesh
	//	//	aiMesh* mesh = scene->mMeshes[i];
	//	//	//Apply bone info to vertices
	//	//	if (mesh->HasBones())
	//	//	{
	//	//		std::vector<Bone> boneList;

	//	//		for (int bi = 0; bi < mesh->mNumBones; bi++)
	//	//		{
	//	//			aiBone* bone = mesh->mBones[bi];

	//	//			Bone derp_bone;
	//	//			derp_bone.name = bone->mName.C_Str();
	//	//			derp_bone.offset = aiMatrix4x4ToGlm(&bone->mOffsetMatrix);
	//	//			boneList.push_back(derp_bone);
	//	//		}

	//	//		aiAnimation* aiAnim = scene->mAnimations[0];
	//	//		Animation anim;
	//	//		anim.duration = aiAnim->mDuration;
	//	//		anim.ticksPerSecond = aiAnim->mTicksPerSecond;

	//	//		NodeAnim* root = buildTree(scene->mRootNode, nullptr);
	//	//		loadMovementData(root, aiAnim);
	//	//		anim.root = root;

	//	//		animBoneMap.insert({ animCounter, boneList });
	//	//		animIDAnimationMap.insert({ animCounter, anim });
	//	//		animCounter++;
	//	//	}
	//	//}
	//	return 0;
	//}
}