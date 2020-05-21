#pragma once

//Only included by the user software

//Includes Application
#include "Debug/ApplicationDebug.h"
#include "Debug/ApplicationEditor.h"

//Core
#include "core/Application.h"
#include "core/MeshManager.h"

//Entity Component System
#include "core/ecs/Component.h"
#include "core/ecs/ComponentManager.h"
#include "core/ecs/Entity.h"
#include "core/ecs/EntityManager.h"

//Components
#include "core/ecs/components/Transform.h"
#include "core/ecs/components/Script.h"
#include "core/ecs/components/ComponentMesh.h"
#include "core/ecs/components/ComponentMaterial.h"

//Debug Includes


//Defines
//#define EntityManager EntityManager::getInstance()
//#define ComponentManager ComponentManager::getInstance()
//#define Transform Transform::getInstance()