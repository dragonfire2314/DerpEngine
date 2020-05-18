#include <DerpEngine.h>
#include <stdio.h>

#include "Movement.h"

using namespace DERP;

int main() 
{
	//Creating entites
	Entity* ent = EntityManager::getInstance().createEntity();
	Entity* ent2 = EntityManager::getInstance().createEntity();

	//Interfaces
	Application app;
	ComponentManager componentManager;

	MeshManager meshManage;

	//Loading a mesh
	int temp = meshManage.loadMeshes("../models/handmade/triangle.obj");

	if (temp == -1) 
	{
		printf("Bad Dir");
	}

	//Adding transforms
	componentManager.addComponent(ComponentTransform::getInstance(), ent);
	//Adding scripts
	componentManager.addScript(new Movement(), ent);

	//Adding a mesh
	componentManager.addComponent(ComponentMesh::getInstance(), ent);
	Mesh* m = ent->getComponent<Mesh>(ComponentMesh::getInstance());
	m->mesh = meshManage.getMesh(0,0);

	//Adding a material
	componentManager.addComponent(ComponentMaterial::getInstance(), ent);
	Material* mat = ent->getComponent<Material>(ComponentMaterial::getInstance());
	mat->setShader("vertex.v", "pixel.p");


	app.Run();

	return 0;
}