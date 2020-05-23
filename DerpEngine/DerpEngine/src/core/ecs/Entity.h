#pragma once

#include <bitset>
#include <vector>

class Entity 
{
private:

public:
	std::vector<uint32_t> childern;
	Entity* parent;

	std::bitset<UINT8_MAX> components;
	uint32_t ID;
};