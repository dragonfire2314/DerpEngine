#pragma once

#include <vector>
#include <unordered_map>

namespace DERP 
{
	class ComponentBase 
	{
	private:

	public:

	};

	template <typename T>
	class Component : public ComponentBase 
	{
	private:
		std::vector<T> componentArray;
		//Key - EntityID
		//Value - Index to array
		std::unordered_map<uint32_t, uint32_t> entityToComponent;
	public:
		void addData(uint32_t entityID, T component)
		{
			componentArray.push_back(component);
			entityToComponent.insert({ entityID, componentArray.size() - 1 });
		}

		T* getData(uint32_t entityID)
		{
			return &componentArray[entityToComponent[entityID]];
		}

		std::vector<T> getArray() 
		{
			return componentArray;
		}
	};
}