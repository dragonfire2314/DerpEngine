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
		if (Input::getMouseHeld(DERP_MOUSE_BUTTON_2)) {

			Input::setCursorMode(DERP_CURSOR_DISABLED);

			t->rotation = glm::angleAxis((float)glm::radians(-Input::getMouseAxisX()), glm::vec3(0.0f, 1.0f, 0.0f));
			t->rotation *= glm::angleAxis((float)glm::radians(Input::getMouseAxisY()), glm::vec3(1.0f, 0.0f, 0.0f));

			glm::vec3 forward = t->rotation * glm::vec3(0, 0, -1);
			glm::vec3 up = t->rotation * glm::vec3(0, 1, 0);

			const float cameraSpeed = 2.5f * TIME::deltaTime;
			if (Input::getKeyHeld(DERP_KEY_W)) {
				t->position += cameraSpeed * forward;
			}
			if (Input::getKeyHeld(DERP_KEY_S)) {
				t->position -= cameraSpeed * forward;
			}
			if (Input::getKeyHeld(DERP_KEY_A)) {
				t->position -= glm::normalize(glm::cross(forward, up)) * cameraSpeed;
			}
			if (Input::getKeyHeld(DERP_KEY_D)) {
				t->position += glm::normalize(glm::cross(forward, up)) * cameraSpeed;
			}
		}
		else 
		{
			Input::resetMouse();
			Input::setCursorMode(DERP_CURSOR_NORMAL);
		}
	}
};