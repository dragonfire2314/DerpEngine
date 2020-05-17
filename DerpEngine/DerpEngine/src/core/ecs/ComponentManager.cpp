#include "ComponentManager.h"

namespace DERP {

	ComponentManager::ComponentManager() 
	{
		printf("Component Manager Start\n");
	}

	ComponentManager::~ComponentManager() 
	{

	}

	void ComponentManager::addComponent(Component* comp, Entity* e)
	{
		comp->addComponent(e->getID());
		e->addComponent(comp);
	}

	void ComponentManager::addScript(Script* script, Entity* e)
	{
		ComponentScript::getInstance()->addScript(e->getID(), script);
	}

}