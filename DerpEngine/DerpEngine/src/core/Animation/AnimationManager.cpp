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
	//Key animID;
	//Value bone vector
	std::unordered_map<uint32_t, std::vector<Bone>> animBoneMap;
	//Key animID;
	//Value animation
	std::unordered_map<uint32_t, Animation> animIDAnimationMap;

	/*
		TEMP
	*/
	//Key animID;
	//Value bone vector
	std::unordered_map<uint32_t, std::vector<aiBone*>> AI_animToBoneMap;
	//Key animID;
	//Value animation
	std::unordered_map<uint32_t, aiAnimation*> AI_animToAnimationMap;
	//Key animID;
	//Value root node
	std::unordered_map<uint32_t, aiNode*> AI_animToNodeMap;
	/*
		END TEMP
	*/


	uint32_t animCounter = 0;

	//Helper Function
	glm::mat4 ANIM_aiMatrix4x4ToGlm(const aiMatrix4x4* from)
	{
		glm::mat4 to;

		to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
		to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
		to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
		to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;

		return to;
	}

	glm::vec3 ANIM_aiVector3dToGlm(const aiVector3D* from)
	{
		glm::vec3 to;

		to.x = from->x; 
		to.y = from->y;
		to.z = from->z;

		return to;
	}

	glm::quat ANIM_aiQuaternionToGlm(const aiQuaternion* from)
	{
		glm::quat to;

		to.x = from->x;
		to.y = from->y;
		to.z = from->z;
		to.w = from->w;

		return to;
	}

	NodeAnim* buildTree(aiNode* node, NodeAnim* parent)
	{
		//Create a node and set its parent and the parents children
		NodeAnim* nodeAnim = new NodeAnim();
		nodeAnim->parent = parent;
		//Copy data from node
		nodeAnim->name = node->mName.C_Str();
		nodeAnim->transformMat = ANIM_aiMatrix4x4ToGlm(&node->mTransformation);

		for (int y = 0; y < node->mNumChildren; y++) {
			nodeAnim->Children.push_back(buildTree(node->mChildren[y], nodeAnim));
		}

		return nodeAnim;
	}

	void printAiNode(aiNode* node)
	{
		printf("aiNode: %s\n", node->mName.C_Str());

		for (int y = 0; y < node->mNumChildren; y++)
			printAiNode(node->mChildren[y]);
	}

	void printNodeAnim(NodeAnim* node)
	{
		printf("NodeAn: %s\n", node->name.c_str());

		for (int y = 0; y < node->Children.size(); y++)
			printNodeAnim(node->Children[y]);
	}

	void loadMovementData(NodeAnim* node, aiAnimation* ai)
	{
		for (int ia = 0; ia < ai->mNumChannels; ia++) {
			if (node->name == ai->mChannels[ia]->mNodeName.C_Str()) {
				aiNodeAnim* aiNA = ai->mChannels[ia];
				//Position
				node->position = new glm::vec3[aiNA->mNumPositionKeys];
				for (int ip = 0; ip < aiNA->mNumPositionKeys; ip++) {
					node->position[ip] = ANIM_aiVector3dToGlm(&aiNA->mPositionKeys[0].mValue);
				}
				//Rotation
				node->rotation = new glm::quat[aiNA->mNumRotationKeys];
				for (int ir = 0; ir < aiNA->mNumRotationKeys; ir++) {
					node->rotation[ir] = ANIM_aiQuaternionToGlm(&aiNA->mRotationKeys[0].mValue);
				}
				//Scale
				node->scale = new glm::vec3[aiNA->mNumScalingKeys];
				for (int is = 0; is < aiNA->mNumScalingKeys; is++) {
					node->scale[is] = ANIM_aiVector3dToGlm(&aiNA->mScalingKeys[0].mValue);
				}
			}
		}

		for (int y = 0; y < node->Children.size(); y++)
			loadMovementData(node->Children[y], ai);
	}






	aiNodeAnim* findNodeAnim(aiAnimation* anim, std::string value) 
	{
		for (int i = 0; i < anim->mNumChannels; i++) 
		{
			aiNodeAnim* node = anim->mChannels[i];
			if (node->mNodeName.C_Str() == value)
				return node;
		}
	}

	void BoneUpdater(aiNode* node, uint32_t x, aiAnimation* anim)
	{
		//Do stuff with node
		Mesh* mesh = CM::GetComponent<Mesh>(x);
		uint32_t boneID = mesh->mesh->boneNameToID[std::string(node->mName.C_Str())];

		if (node->mParent == nullptr) {

		}
		else {
			mesh->mesh->bones[boneID].offset = ANIM_aiMatrix4x4ToGlm(&node->mParent->mTransformation) * ANIM_aiMatrix4x4ToGlm(&node->mTransformation);
		}

		//Go to children nodes
		for (int y = 0; y < node->mNumChildren; y++)
			BoneUpdater(node->mChildren[y], x, anim);
	}

	void updateAnimations() 
	{
		for (auto x : sys_animator->Entities) 
		{
			aiAnimation* anim = AI_animToAnimationMap[0];

			BoneUpdater(AI_animToNodeMap[0], x, anim);
		}
	}

	int loadAnimationFromFile(std::string path)
	{
		const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		if (!scene) {
			std::cerr << "Could not load file " << path << aiGetErrorString() << std::endl;
			return -1;
		}
		/*
			SLOW CODE
		*/
		aiAnimation* aiAnim = scene->mAnimations[0];
		aiNode* node = scene->mRootNode;

		AI_animToAnimationMap.insert({ animCounter, aiAnim });
		AI_animToNodeMap.insert({ animCounter, node });
		animCounter++;

		/*
			PROBABLY BETTER CODE, ONLY TRY ONCE SLOW CODE WORKS
		*/
		//for (int i = 0; i < scene->mNumMeshes; i++)
		//{
		//	//Generate aiMesh
		//	aiMesh* mesh = scene->mMeshes[i];
		//	//Apply bone info to vertices
		//	if (mesh->HasBones())
		//	{
		//		std::vector<Bone> boneList;

		//		for (int bi = 0; bi < mesh->mNumBones; bi++)
		//		{
		//			aiBone* bone = mesh->mBones[bi];

		//			Bone derp_bone;
		//			derp_bone.name = bone->mName.C_Str();
		//			derp_bone.offset = aiMatrix4x4ToGlm(&bone->mOffsetMatrix);
		//			boneList.push_back(derp_bone);
		//		}

		//		aiAnimation* aiAnim = scene->mAnimations[0];
		//		Animation anim;
		//		anim.duration = aiAnim->mDuration;
		//		anim.ticksPerSecond = aiAnim->mTicksPerSecond;

		//		NodeAnim* root = buildTree(scene->mRootNode, nullptr);
		//		loadMovementData(root, aiAnim);
		//		anim.root = root;

		//		animBoneMap.insert({ animCounter, boneList });
		//		animIDAnimationMap.insert({ animCounter, anim });
		//		animCounter++;
		//	}
		//}
		return 0;
	}
}