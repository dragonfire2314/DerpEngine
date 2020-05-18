#pragma once

#include <DerpEngine.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <chrono> 

#include "../DerpEngine/external/glm/glm.hpp"
#include "../DerpEngine/external/glm/gtc/matrix_transform.hpp"
#include "../DerpEngine/external/glm/gtc/type_ptr.hpp"

using namespace DERP;

class Movement : public Script
{
private:

public:

	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;

	Transform *t;

	std::chrono::high_resolution_clock::time_point t0;

	void Start() 
	{
		t0 = Time::now();

		std::srand(std::time(nullptr));

		printf("Script start was called!!!!!!!!!!!!!!!!!!!\n");

		t = getComponent<Transform>(ComponentTransform::getInstance());

		t->position = glm::vec3(0, 0, 0);

		t->rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void Update() 
	{
		//t->position = 
		//	glm::vec3(t->position.x + ((std::rand() % 100) / 100.0f), 1, 0);

		auto t1 = Time::now();
		fsec fs = t1 - t0;
		ms d = std::chrono::duration_cast<ms>(fs);

		t->rotation = glm::angleAxis((float)glm::sin(glm::radians((float)d.count())) * 0.05f, glm::vec3(0.0f, 0.0f, 1.0f));

		//printf("Test Position %f, %f, %f\n",
		//	t->position.x,
		//	t->position.y,
		//	t->position.z);
	}
};

