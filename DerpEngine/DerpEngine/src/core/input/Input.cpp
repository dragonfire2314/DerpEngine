#include "Input.h"

namespace DERP 
{
	uint16_t Input::keyHeld[348];
	uint16_t Input::keyUp[348];
	uint16_t Input::keyDown[348];

	//void Input::Init(GLFWwindow* window)
	//{
	//	//Generate key call back
	//	glfwSetKeyCallback(window, key_callback);
	//	//Make sure arrays are cleared - TODO
	//}

	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	//{
	//	if (action == GLFW_PRESS) 
	//	{
	//		keyDown[8] = 0;
	//	}
	//}

	bool Input::getKeyUp(uint8_t key) { return false; }
	bool Input::getKeyDown(uint8_t key) { return false; }
	bool Input::getKeyHeld(uint8_t key) { return false; }

	bool Input::getMouseUp(uint8_t key) { return false; }
	bool Input::getMouseDown(uint8_t key) { return false; }
	bool Input::getMouseHeld(uint8_t key) { return false; }
}