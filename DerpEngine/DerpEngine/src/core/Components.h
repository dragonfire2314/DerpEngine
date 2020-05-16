#pragma once

#include "core.h"

#include "ComponentStructs.h"

#include <vector>
#include <memory>

/*
This file contains all compont queue definitions and must be updated when a new component is added

The areas that should be updated are as follows
	-Components enum
	-Component stuct must be added
	-Component vector must be added (Component Manger)
	-Component count should be incramented
*/

#define COMPONENT_COUNT = 5;

//DO NOT ASSIGN VALUES TO ENTRIES, ONLY ZERO SHOULD BE ASSIGNED
enum DERP_API Components {
	TRANSFORM = 0,
	MESH,
	MATERIAL,
	RIGIDBODY,
	SCRIPT
};
