#include <DerpEngine.h>
#include <stdio.h>

#include "Movement.h"

using namespace DERP;

int main() 
{
	Entity* ent = EntityManager::getInstance().createEntity();
	Entity* ent2 = EntityManager::getInstance().createEntity();

	ApplicationDebug app;
	ComponentManager componentManager;

	componentManager.addComponent(ComponentTransform::getInstance(), ent);
	componentManager.addComponent(ComponentTransform::getInstance(), ent2);

	componentManager.addScript(new Movement(), ent);
	componentManager.addScript(new Movement(), ent2);

	componentManager.addComponent(ComponentMesh::getInstance(), ent);

	//ent->getComponent<Mesh>(ComponentMesh::getInstance())->mesh = ;

	app.Run();

	return 0;
}