#include "Input.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

namespace DERP 
{
	GLFWwindow* window;

    bool firstMouse = true;
    float lastX = 400, lastY = 300;
    float yaw = 0, pitch = 0, roll = 0;

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;
	}

	void Input::Init(void* _window)
	{
		window = (GLFWwindow*)_window;
		glfwSetCursorPosCallback(window, cursor_position_callback);
	}

    //Temp
    void Input::resetMouse() { firstMouse = true; }

	bool Input::getKeyUp(uint16_t key) { 
		return false;
	}
	bool Input::getKeyDown(uint16_t key) { 
		return false;
	}
	bool Input::getKeyHeld(uint16_t key) { 
		return (glfwGetKey(window, key) == GLFW_PRESS);
	}

	bool Input::getMouseUp(uint16_t key) { return false; }
	bool Input::getMouseDown(uint16_t key) { return false; }
	bool Input::getMouseHeld(uint16_t key) { 
        return (glfwGetMouseButton(window, key) == GLFW_PRESS);
    }

    float Input::getMouseAxisX()
    {
        return yaw;
    }

    float Input::getMouseAxisY()
    {
        return pitch;
    }

    void Input::setCursorMode(uint32_t type)
    {
        glfwSetInputMode(window, GLFW_CURSOR, type);
    }
}