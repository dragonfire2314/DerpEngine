#pragma once

#include "../ecs/Components.h"

namespace DERP 
{
	struct ColliderTransformRigidBodyPair
	{
		Collider* c;
		Transform* t;
		RigidBody* r;
	};

	struct Manifold 
	{
		Transform* at;
		Transform* bt;

		RigidBody* ar;
		RigidBody* br;

		glm::vec3 normalDir;

		float depth = 0;

		bool isCollision;
	};

	Manifold AABBvsAABB(ColliderTransformRigidBodyPair a, ColliderTransformRigidBodyPair b);
	Manifold AABBvsSphere(ColliderTransformRigidBodyPair a, ColliderTransformRigidBodyPair b);
	Manifold SpherevsSphere(ColliderTransformRigidBodyPair a, ColliderTransformRigidBodyPair b);
}