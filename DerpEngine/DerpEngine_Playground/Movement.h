#pragma once

#include <DerpEngine.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

using namespace DERP;

class Movement : public Script
{
private:

public:

	Transform *t;

	void Start() 
	{
		std::srand(std::time(nullptr));

		printf("Script start was called!!!!!!!!!!!!!!!!!!!\n");

		t = getComponent<Transform>(ComponentTransform::getInstance());

		t->position = glm::vec3(0, 1, 0);

		
	}

	void Update() 
	{
		t->position = 
			glm::vec3(t->position.x + ((std::rand() % 100) / 100.0f), 1, 0);

		//printf("Test Position %f, %f, %f\n",
		//	t->position.x,
		//	t->position.y,
		//	t->position.z);
	}
};

