#include "AnimationManager.h"

#include "AnimationDataTypes.h"

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

	NodeAnim* buildTree(aiNode* node, NodeAnim* parent)
	{
		//Create a node and set its parent and the parents children
		NodeAnim* nodeAnim = new NodeAnim();
		nodeAnim->parent = parent;
		//Copy data from node
		nodeAnim->name = node->mName.C_Str();
		nodeAnim->transformMat = aiMatrix4x4ToGlm(&node->mTransformation);

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
					node->position[ip] = aiVector3dToGlm(&aiNA->mPositionKeys[0].mValue);
				}
				//Rotation
				node->rotation = new glm::quat[aiNA->mNumRotationKeys];
				for (int ir = 0; ir < aiNA->mNumRotationKeys; ir++) {
					node->rotation[ir] = aiQuaternionToGlm(&aiNA->mRotationKeys[0].mValue);
				}
				//Scale
				node->scale = new glm::vec3[aiNA->mNumScalingKeys];
				for (int is = 0; is < aiNA->mNumScalingKeys; is++) {
					node->scale[is] = aiVector3dToGlm(&aiNA->mScalingKeys[0].mValue);
				}
			}
		}

		for (int y = 0; y < node->Children.size(); y++)
			loadMovementData(node->Children[y], ai);
	}

	int loadAnimationFromFile(std::string path) 
	{
		const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		if (!scene) {
			std::cerr << "Could not load file (anim) " << path << aiGetErrorString() << std::endl;
			return -1;
		}

		printf("There are %i meshes in the animation import", scene->mNumMeshes);

		/*
			SLOW CODE
		*/
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//Generate aiMesh
			aiMesh* mesh = scene->mMeshes[i];
			//Apply bone info to vertices
			if (mesh->HasBones())
			{
				std::vector<aiBone*> boneList;

				for (int bi = 0; bi < mesh->mNumBones; bi++)
				{
					aiBone* bone = mesh->mBones[bi];
					boneList.push_back(bone);
				}

				aiAnimation* aiAnim = scene->mAnimations[0];
				aiNode* node = scene->mRootNode;

				AI_animToBoneMap.insert({ animCounter, boneList });
				AI_animToAnimationMap.insert({ animCounter, aiAnim });
				AI_animToNodeMap.insert({ animCounter, node });
				animCounter++;
			}
		}

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
	}
}


/*for (int vi = 0; vi < bone->mNumWeights; vi++)
					{
						unsigned int vID = bone->mWeights[vi].mVertexId;

						if (verts[vID].BoneID.x == -1)
							verts[vID].BoneID.x = bi;
						else if (verts[vID].BoneID.y == -1)
							verts[vID].BoneID.y = bi;
						else if (verts[vID].BoneID.z == -1)
							verts[vID].BoneID.z = bi;
						else if (verts[vID].BoneID.w == -1)
							verts[vID].BoneID.w = bi;
						else
							printf("ERROR: Out of bone ID spots");


						float weight = bone->mWeights[vi].mWeight;

						if (verts[vID].BoneWeights.x == -1)
							verts[vID].BoneWeights.x = weight;
						else if (verts[vID].BoneWeights.y == -1)
							verts[vID].BoneWeights.y = weight;
						else if (verts[vID].BoneWeights.z == -1)
							verts[vID].BoneWeights.z = weight;
						else if (verts[vID].BoneWeights.w == -1)
							verts[vID].BoneWeights.w = weight;
						else
							printf("ERROR: Out of bone weight spots");
					}*/