#pragma once

#include <DerpEngine.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <chrono> 
#include <math.h>

#include "../DerpEngine/external/glm/glm.hpp"
#include "../DerpEngine/external/glm/gtc/matrix_transform.hpp"
#include "../DerpEngine/external/glm/gtc/type_ptr.hpp"

using namespace DERP;

class RotateAroundScene : public Scriptable
{
private:

public:

	Transform* t;

	float PI = 3.14159265;

	void Start()
	{
		printf("Rotation Script!\n");

		t = getComponent<Transform>();
	}

	float counter = 0;

	void Update()
	{
		counter += 1;

		t->position.x = std::sin(counter * PI / 180) * 3;
		t->position.z = std::cos(counter * PI / 180) * 3;
	}
};