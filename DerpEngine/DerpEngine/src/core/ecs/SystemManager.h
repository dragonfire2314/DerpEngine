#pragma once
#include "../core.h"

#include <assert.h>
#include <unordered_map>
#include <bitset>
#include <vector>

#include "systems/System.h"

namespace DERP 
{
	class DERP_API SystemManager 
	{
	private:
		//Key - typeName
		//Value - 
		static std::unordered_map<const char*, std::bitset<UINT8_MAX>> Signatures;
		static std::unordered_map<const char*, System*> Systems;
	public:
		template<typename T>
		static T* RegisterSystem()
		{
			const char* typeName = typeid(T).name();

			assert(Systems.find(typeName) == Systems.end() && "Already registered");

			T* system = new T();
			Systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		static void SetSignature(std::bitset<UINT8_MAX> signature)
		{
			const char* typeName = typeid(T).name();

			assert(Systems.find(typeName) != Systems.end() && "System not registered");

			Signatures.insert({ typeName, signature });
		}

		static void EntitySignatureChanged(uint32_t entityID, std::bitset<UINT8_MAX> entitySignature)
		{
			for (auto x : Systems)
			{
				auto systemSignature = Signatures[x.first];

				if (std::find(x.second->Entities.begin(), x.second->Entities.end(), entityID) != x.second->Entities.end()) {
					//The system already has this entity
				}
				else {
					if ((entitySignature & systemSignature) == systemSignature)
					{
						x.second->Entities.push_back(entityID);
					}
				}
			}
		}
	};
}