#include "Physics.h"

#include "../ecs/ComponentManager.h"
#include "../ecs/Components.h"
#include "../ecs/systems/System.h"
#include "../time/Time.h"

#include "../collision/Collider.h"

namespace DERP
{
	namespace PHYS
	{
		void resolveCollision(Manifold m);

		void integrate(float dt) 
		{
			for (auto x : sys_physics->Entities) 
			{
				RigidBody* rb = CM::GetComponent<RigidBody>(x);
				Transform* t = CM::GetComponent<Transform>(x);

				//Apply Gravity
				if (rb->useGravity)
					rb->acceleration = glm::vec3(0, rb->gravityAmount, 0);

				//Integrate veclocity
				rb->velocity += rb->acceleration * dt;
				//Integrate Position
				t->position += rb->velocity * dt;
			}
		}

		void checkCollisions() 
		{
			//Pull a list of all the objects with collision detectors on rigidboies
			std::vector<ColliderTransformRigidBodyPair> colliders;
			colliders.reserve(sys_boxCollider->Entities.size() + sys_sphereCollider->Entities.size());
			for (auto rb : sys_physics->Entities) {
				for (auto x : sys_boxCollider->Entities) {
					if (x != rb) continue; //TODO - Optomize this nightmare code
					ColliderTransformRigidBodyPair e;
					e.r = CM::GetComponent<RigidBody>(x);
					e.c = CM::GetComponent<BoxCollider>(x);
					e.t = CM::GetComponent<Transform>(x);
					assert(e.c->type != NONE); //Make sure the collider has a type
					colliders.push_back(e);
				}
				for (auto x : sys_sphereCollider->Entities) {
					if (x != rb) continue; //TODO - Optomize this nightmare code
					ColliderTransformRigidBodyPair e;
					e.r = CM::GetComponent<RigidBody>(x);
					e.c = CM::GetComponent<SphereCollider>(x);
					e.t = CM::GetComponent<Transform>(x);
					assert(e.c->type != NONE); //Make sure the collider has a type
					colliders.push_back(e);
				}
			}

			//Collide all colliders
			/* Only collides everthing one time
			e.g.
				0-1  1-2  2-3
				0-2  1-3
				0-3
			*/
			for (int i = 0; i < colliders.size() - 1; i++)
			{
				for (int j = i + 1; j < colliders.size(); j++)
				{
					Manifold manifold;
					//Determine which of the 3 collision functions to call
					if (colliders[i].c->type == SPHERE && colliders[j].c->type == SPHERE)
						manifold = SpherevsSphere(colliders[i], colliders[j]);
					else if (colliders[i].c->type == BOX && colliders[j].c->type == BOX)
						manifold = AABBvsAABB(colliders[i], colliders[j]);
					else {
						if (colliders[i].c->type == BOX)
							manifold = AABBvsSphere(colliders[i], colliders[j]);
						else {
							manifold = AABBvsSphere(colliders[j], colliders[i]);
						}
					}
					//Deal with manifold
					resolveCollision(manifold);
				}
			}
		}

		void resolveCollision(Manifold m) 
		{
			//Check if a collision happened
			if (!m.isCollision) return;

			//Find j
			glm::vec3 rv = m.br->velocity - m.ar->velocity;
			float dotWithNorm = glm::dot(rv, m.normalDir);

			if (dotWithNorm > 0) //Objects already moving apart
				return;

			float e = 0.5;

			float eFixed = -(1 + e);

			float j = eFixed * dotWithNorm;
			j /= (m.ar->inverseMass + m.br->inverseMass);

			//Impulse
			glm::vec3 impulse = m.normalDir * j;
			m.ar->velocity -= m.ar->inverseMass * impulse;
			m.br->velocity += m.br->inverseMass * impulse;

			//Correct for floating point errors
			float percent = 0.2;
			float slop = 0.01;
			glm::vec3 Correction = glm::max(m.depth - slop, 0.0f ) / (m.ar->inverseMass + m.br->inverseMass) * percent * m.normalDir;
			m.at->position -= m.ar->inverseMass * Correction;
			m.bt->position += m.br->inverseMass * Correction;
		}

		void updatePhysics()
		{
			//Run 1/30 second phyics if the frame time gets too large
			if (Time::deltaTime > (1.0f / 20.0f))
				integrate((1.0f / 30.0f));
			else
				integrate(Time::deltaTime);

			checkCollisions();
		}
	}
}