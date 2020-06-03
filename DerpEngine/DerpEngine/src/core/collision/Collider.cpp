#include "Collider.h"

#include "../ecs/systems/System.h"
#include "../ecs/ComponentManager.h"

#include <vector>
#include <algorithm>

namespace DERP
{
	Manifold AABBvsAABB(ColliderTransformRigidBodyPair a, ColliderTransformRigidBodyPair b)
	{
		BoxCollider* ca = (BoxCollider*)a.c;
		BoxCollider* cb = (BoxCollider*)b.c;

		Manifold m;
		
		m.at = a.t;
		m.bt = b.t;

		m.ar = a.r;
		m.br = b.r;

		m.isCollision = false;

		glm::vec3 AtoB = b.t->position - a.t->position;

		float x_overlap = ca->halfSides.x + cb->halfSides.x - glm::abs(AtoB.x);

		if (x_overlap > 0) 
		{
			float z_overlap = ca->halfSides.z + cb->halfSides.z - glm::abs(AtoB.z);

			if (z_overlap > 0)
			{
				float y_overlap = ca->halfSides.y + cb->halfSides.y - glm::abs(AtoB.y);

				if (y_overlap > 0)
				{
					//Collision
					m.isCollision = true;
					//Find axis of least penatration
					if (x_overlap < z_overlap && x_overlap < y_overlap) { //X Axis
						m.depth = x_overlap;
						if (AtoB.x < 0)
							m.normalDir = glm::vec3(-1, 0, 0);
						else
							m.normalDir = glm::vec3(1, 0, 0);
					}
					if (y_overlap < x_overlap && y_overlap < z_overlap) { //Y Axis
						m.depth = y_overlap;
						if (AtoB.y < 0)
							m.normalDir = glm::vec3(0, -1, 0);
						else
							m.normalDir = glm::vec3(0, 1, 0);
					}
					if (z_overlap < x_overlap && z_overlap < y_overlap) { //Z Axis
						m.depth = z_overlap;
						if (AtoB.z < 0)
							m.normalDir = glm::vec3(0, 0, -1);
						else
							m.normalDir = glm::vec3(0, 0, 1);
					}
				}
			}
		}

		return m;
	}

	Manifold AABBvsSphere(ColliderTransformRigidBodyPair a, ColliderTransformRigidBodyPair b)
	{
		return Manifold();
	}

	Manifold SpherevsSphere(ColliderTransformRigidBodyPair a, ColliderTransformRigidBodyPair b)
	{
		return Manifold();
	}
}