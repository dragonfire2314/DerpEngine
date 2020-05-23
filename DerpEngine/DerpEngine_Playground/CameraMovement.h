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

class CameraMovement : public Scriptable
{
private:

public:

	Transform* t;

	void Start()
	{
		printf("Camera Script!\n");

		t = getComponent<Transform>();
	}

	void Update()
	{

	}
};

#pragma once
