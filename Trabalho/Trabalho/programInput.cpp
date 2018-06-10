#include "programInput.h"
#include <GLFW\glfw3.h>  
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <iostream>

double _camZoom = 0;
double _xOffset, _yOffset;
float angleFlow = 0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    _camZoom += yoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)//ambient light
		ambientLightOn = (ambientLightOn == 0) ? 1 : 0;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)//directional light
		dirLightOn = (dirLightOn == 0) ? 1 : 0;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)//point light
		pointLightOn = (pointLightOn == 0) ? 1 : 0;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)//spot light
        spotLightOn = (spotLightOn == 0) ? 1 : 0;
}   

void update(GLFWwindow* window)
{
    glfwGetCursorPos(window, &_xOffset, &_yOffset);

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    double cx = w/2, cy = h/2;

    //changed the coordinates so middle is 0,0
    _xOffset = _xOffset - cx;
    _yOffset = _yOffset - cy;

    //create the matrix for the rotation
    //angle change rate
    float xAngle, yAngle;
}

void inputOnce(GLFWwindow* window)
{
    glfwSetScrollCallback(window, scroll_callback); // scroll wheel
    glfwSetKeyCallback(window, key_callback); // keyboard
}

glm::mat4 updateZoom()
{
    glm::mat4 newProjection = glm::perspective(glm::radians(45.0f + (float)_camZoom), 16.0f/9.0f, 0.1f, 100.0f); 
    printf("a");
    return newProjection;
}

glm::mat4 updateRotation()
{
    glm::mat4 newView = glm::rotate(glm::mat4(), angleFlow += 0.02f, glm::normalize(glm::vec3(_xOffset, _yOffset, 0)));
    printf("b");
    return newView;
}