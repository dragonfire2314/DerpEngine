#include <DerpEngine.h>
#include <stdio.h>
#include <iostream>

#include "Movement.h"
#include "CameraMovement.h"

using namespace DERP;

int main()
{
	ComponentManager cm;
	SystemManager sm;

	Application app;

	app.Init();
	EntityManager::Init();

	//Creating entites
	uint32_t e = EntityManager::CreateEntity();
	uint32_t e2 = EntityManager::CreateEntity();

	printf("e, e2 %i, %i\n", e, e2);

	MeshManager meshManager;
	int temp = meshManager.loadMeshes("../models/Treasure_Chest/treasure_chest2.obj");
	if (temp == -1) 
	{
		printf("Bad Dir");
	}

	int temp22 = meshManager.loadMeshes("../models/Chemical_Barrel/chemicalbarrel.obj");
	if (temp22 == -1)
	{
		printf("Bad Dir");
	}

	cm.AddComponent<Transform>(e);

	cm.AddComponent<Mesh>(e);
	Mesh* m = cm.GetComponent<Mesh>(e);
	m->mesh = meshManager.getMesh(0, 0);

	cm.AddComponent<Material>(e);
	Material* mat = cm.GetComponent<Material>(e);
	mat->mat = meshManager.getMaterial(0, 0);
	
	mat->setShader("vertex.v", "pixel.p");

	//cm.AddComponent<Script>(e);
	//Script* s = ComponentManager::GetComponent<Script>(e);
	//s->script = new Movement();





	cm.AddComponent<Transform>(e2);
	Transform* t_barrel = cm.GetComponent<Transform>(2);
	t_barrel->position.z = -10;

	cm.AddComponent<Mesh>(e2);
	Mesh* m2 = cm.GetComponent<Mesh>(e2);
	m2->mesh = meshManager.getMesh(1, 0);

	cm.AddComponent<Material>(e2);
	Material* mat2 = cm.GetComponent<Material>(e2);
	mat2->mat = meshManager.getMaterial(1, 0);

	mat2->setShader("vertex.v", "pixel.p");

	//cm.AddComponent<Script>(e2);
	//Script* s2 = ComponentManager::GetComponent<Script>(e2);
	//s2->script = new Movement();





	//Camera Entity
	uint32_t cam = EntityManager::CreateEntity();
	ComponentManager::AddComponent<Camera>(cam);
	ComponentManager::AddComponent<Transform>(cam);
	Transform* cam_t = ComponentManager::GetComponent<Transform>(cam);
	cam_t->position.z = 4;
	cm.AddComponent<Script>(cam);
	Script* s3 = ComponentManager::GetComponent<Script>(cam);
	s3->script = new CameraMovement();

	app.Run();

	return 0;
}




//#include <DerpEngine.h>
//#include <stdio.h>
//
//#include "Movement.h"
//
//using namespace DERP;
//
//int main() 
//{
//	//Creating entites
//	Entity* ent = EntityManager::getInstance().createEntity();
//	Entity* ent2 = EntityManager::getInstance().createEntity(ent);
//
//	//Interfaces
//	ApplicationDebug app;
//	ComponentManager componentManager;
//
//	MeshManager meshManage;
//
//	//Loading a mesh
//	int temp = meshManage.loadMeshes("../models/Treasure_Chest/treasure_chest2.obj");
//
//	if (temp == -1) 
//	{
//		printf("Bad Dir");
//	}
//
//	//Adding transforms
//	componentManager.addComponent(ComponentTransform::getInstance(), ent);
//	//componentManager.addComponent(ComponentTransform::getInstance(), ent2);
//	//Transform* ent2_t = componentManager.getComponent<Transform>(ComponentTransform::getInstance(), ent2);
//	//ent2_t->position = glm::vec3(1, 0, -2);
//	//Adding scripts
//	componentManager.addScript(new Movement(), ent);
//
//	//Adding a mesh
//	componentManager.addComponent(ComponentMesh::getInstance(), ent);
//	Mesh* m = ent->getComponent<Mesh>(ComponentMesh::getInstance());
//	m->mesh = meshManage.getMesh(0,0);
//
//	//componentManager.addComponent(ComponentMesh::getInstance(), ent2);
//	//Mesh* m2 = ent2->getComponent<Mesh>(ComponentMesh::getInstance());
//	//m2->mesh = meshManage.getMesh(0, 0);
//
//	//Adding a material
//	componentManager.addComponent(ComponentMaterial::getInstance(), ent);
//	Material* mat = ent->getComponent<Material>(ComponentMaterial::getInstance());
//	mat->setShader("vertex.v", "pixel.p");
//	mat->mat = meshManage.getMaterial(0, 0);
//
//	//componentManager.addComponent(ComponentMaterial::getInstance(), ent2);
//	//Material* mat2 = ent2->getComponent<Material>(ComponentMaterial::getInstance());
//	//mat2->setShader("vertex.v", "pixel.p");
//
//
//	app.Run();
//
//	return 0;
//}