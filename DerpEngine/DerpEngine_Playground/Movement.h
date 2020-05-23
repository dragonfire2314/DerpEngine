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

class Movement : public Scriptable
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

		printf("Script start was called!\n");

		t = getComponent<Transform>();

		//t->position = glm::vec3(0, 0.5, -2);
	}

	float counter = 0;
	float counter2 = 0;

	void Update() 
	{
		auto t1 = Time::now();
		fsec fs = t1 - t0;
		ms d = std::chrono::duration_cast<ms>(fs);

		counter += 0.5f;
		counter2 += 0.3f;

		t->rotation = glm::angleAxis(glm::radians(counter), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::angleAxis(glm::radians(counter2), glm::vec3(1.0f, 0.0f, 0.0f));
	}
};

