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

class AudioTest : public Scriptable
{
private:

public:

	AudioSource* a;

	void Start()
	{
		printf("Audio test Script!\n");

		a = getComponent<AudioSource>();
	}

	void Update()
	{
		if (TIME::time > 10) 
		{
			printf("Change song\n");
			a->changeAudio("epicSong.ogg");
			a->play();
		}
	}
};