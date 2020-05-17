#include <DerpEngine.h>
#include <stdio.h>

#include "Movement.h"

using namespace DERP;

int main() 
{
	Entity* ent = EntityManager::getInstance().createEntity();
	Entity* ent2 = EntityManager::getInstance().createEntity();

	Application app;
	ComponentManager componentManager;

	MeshManager meshManage;

	int temp = meshManage.loadMeshes("../models/monkey/monkey.obj");

	if (temp == -1) 
	{
		printf("Bad Dir");
	}

	componentManager.addComponent(ComponentTransform::getInstance(), ent);
	componentManager.addComponent(ComponentTransform::getInstance(), ent2);

	componentManager.addScript(new Movement(), ent);
	componentManager.addScript(new Movement(), ent2);

	componentManager.addComponent(ComponentMesh::getInstance(), ent);

	Mesh* m = ent->getComponent<Mesh>(ComponentMesh::getInstance());
	m->mesh = meshManage.getMesh(0,0);

	app.Run();

	return 0;
}