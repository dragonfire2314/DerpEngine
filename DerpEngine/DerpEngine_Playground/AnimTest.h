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

class AnimTest : public Scriptable
{
private:

public:

	Animator* a;

	void Start()
	{
		printf("Audio test Script!\n");

		a = getComponent<Animator>();
	}

	void Update()
	{
		if (TIME::time > 10)
		{
			printf("Change anim\n");
			a->clipID = getAnimationClipID(1, "mixamo.com");
			std::vector<Animator*> child = getComponentInChildern<Animator>();
			for (auto x : child)
				x->clipID = getAnimationClipID(1, "mixamo.com");
		}
	}
};