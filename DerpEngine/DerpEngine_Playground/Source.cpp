#include <DerpEngine.h>
#include <stdio.h>
#include <iostream>

#include "Movement.h"
#include "CameraMovement.h"
#include "RotateAroundScene.h"

using namespace DERP;

int main()
{
	ComponentManager cm;
	SystemManager sm;

	Application app;

	app.Init();
	EntityManager::Init();

    MeshManager meshManager;

    int chest = meshManager.loadMeshes("../models/Treasure_Chest/treasure_chest2.obj");
    int bar = meshManager.loadMeshes("../models/Chemical_Barrel/chemicalbarrel.obj");
    int cube = meshManager.loadMeshes("../models/handmade/cube.obj");
    int ico = meshManager.loadMeshes("../models/handmade/ico.obj");
    int cone = meshManager.loadMeshes("../models/handmade/cone.obj");
    int torus = meshManager.loadMeshes("../models/handmade/torus.obj");
    int plane = meshManager.loadMeshes("../models/handmade/plane.obj");

    //Sample
    uint32_t ent = EntityManager::CreateEntity();
    EntityManager::getEntity(ent)->name = "Cube";
    cm.AddComponent<Transform>(ent);
    cm.AddComponent<Mesh>(ent);
    cm.AddComponent<Material>(ent);
    cm.GetComponent<Mesh>(ent)->mesh = meshManager.getMesh(chest, 0);
    cm.GetComponent<Transform>(ent)->position = glm::vec3(2, -2, 0);
    cm.GetComponent<Material>(ent)->mat = meshManager.getMaterial(chest, 0);

    //Sample 2
    uint32_t ent3 = EntityManager::CreateEntity();
    EntityManager::getEntity(ent3)->name = "Plane";
    cm.AddComponent<Transform>(ent3);
    cm.AddComponent<Mesh>(ent3);
    cm.AddComponent<Material>(ent3);
    cm.GetComponent<Mesh>(ent3)->mesh = meshManager.getMesh(plane, 0);
    cm.GetComponent<Transform>(ent3)->position = glm::vec3(0, -2, 0);
    cm.GetComponent<Material>(ent3)->mat->Kd = objl::Vector3(0.5, 0.1, 0.7);

    //Create a camera
    uint32_t cam = EntityManager::CreateEntity();
    EntityManager::getEntity(cam)->name = "Camera";
    ComponentManager::AddComponent<Camera>(cam);
    ComponentManager::AddComponent<Transform>(cam);
    Transform* cam_t = ComponentManager::GetComponent<Transform>(cam);
    cam_t->position = glm::vec3(-4.44764, 3.3252, 10.8293);
    cam_t->rotation = glm::quat(glm::vec3(-0.278857, -0.310267, -0.0491606));
    cm.AddComponent<Script>(cam);
    Script* s3 = ComponentManager::GetComponent<Script>(cam);
    s3->script = new CameraMovement();

    //Create a Point Light
    uint32_t pLight = EntityManager::CreateEntity();
    EntityManager::getEntity(pLight)->name = "face";
    cm.AddComponent<DirectionalLight>(pLight);
    cm.AddComponent<Transform>(pLight);
    cm.AddComponent<Script>(pLight);
    cm.GetComponent<Script>(pLight)->script = new RotateAroundScene();
    cm.AddComponent<Mesh>(pLight);
    cm.AddComponent<Material>(pLight);
    cm.GetComponent<Mesh>(pLight)->mesh = meshManager.getMesh(ico, 0);
    cm.GetComponent<Transform>(pLight)->position.y = 1;

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