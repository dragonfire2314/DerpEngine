#include <DerpEngine.h>
#include <stdio.h>

#include "Movement.h"

using namespace DERP;

int main() 
{
	//Creating entites
	Entity* ent = EntityManager::getInstance().createEntity();
	Entity* ent2 = EntityManager::getInstance().createEntity(ent);

	//Interfaces
	ApplicationDebug app;
	ComponentManager componentManager;

	MeshManager meshManage;

	//Loading a mesh
	int temp = meshManage.loadMeshes("../models/Treasure_Chest/treasure_chest2.obj");

	if (temp == -1) 
	{
		printf("Bad Dir");
	}

	//Adding transforms
	componentManager.addComponent(ComponentTransform::getInstance(), ent);
	//componentManager.addComponent(ComponentTransform::getInstance(), ent2);
	//Transform* ent2_t = componentManager.getComponent<Transform>(ComponentTransform::getInstance(), ent2);
	//ent2_t->position = glm::vec3(1, 0, -2);
	//Adding scripts
	componentManager.addScript(new Movement(), ent);

	//Adding a mesh
	componentManager.addComponent(ComponentMesh::getInstance(), ent);
	Mesh* m = ent->getComponent<Mesh>(ComponentMesh::getInstance());
	m->mesh = meshManage.getMesh(0,0);

	//componentManager.addComponent(ComponentMesh::getInstance(), ent2);
	//Mesh* m2 = ent2->getComponent<Mesh>(ComponentMesh::getInstance());
	//m2->mesh = meshManage.getMesh(0, 0);

	//Adding a material
	componentManager.addComponent(ComponentMaterial::getInstance(), ent);
	Material* mat = ent->getComponent<Material>(ComponentMaterial::getInstance());
	mat->setShader("vertex.v", "pixel.p");
	mat->mat = meshManage.getMaterial(0, 0);

	//componentManager.addComponent(ComponentMaterial::getInstance(), ent2);
	//Material* mat2 = ent2->getComponent<Material>(ComponentMaterial::getInstance());
	//mat2->setShader("vertex.v", "pixel.p");


	app.Run();

	return 0;
}