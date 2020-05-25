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
		//Should be an array, but the vector that i was using causes my older pointers to data to become devalued,
		//so im switching this to a map to keep that from happening, but proformance at a super high level will be affected
		//std::vector<T> componentArray;
		std::unordered_map<uint32_t, T> componentArray;
		//Key - EntityID
		//Value - Index to array
		std::unordered_map<uint32_t, uint32_t> entityToComponent;
	public:
		void addData(uint32_t entityID, T component)
		{
			//componentArray.push_back(component);
			//entityToComponent.insert({ entityID, componentArray.size() - 1 });

			componentArray.insert({ entityID, component });
		}

		T* getData(uint32_t entityID)
		{
			return &componentArray[entityID];
			//return &componentArray[entityToComponent[entityID]];
		}

		std::vector<T> getArray() 
		{
			return componentArray;
		}
	};
}