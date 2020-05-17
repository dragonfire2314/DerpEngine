#include "ComponentScript.h"

#include "../EntityManager.h"

namespace DERP 
{
	uint32_t ComponentScript::addComponent(uint32_t entityID)
	{
		return 0;
	}

	uint32_t ComponentScript::addScript(uint32_t entityID, Script* script)
	{
		data.insert({ entityID, script });

		script->entity = EntityManager::getInstance().getEntity(entityID);

		return 0;
	}

	void ComponentScript::updateComponent()
	{
		for (auto const& x : data)
		{
			x.second->Update();
		}
	}

	void ComponentScript::startComponent()
	{
		for (auto const& x : data)
		{
			x.second->Start();
		}
	}
}