#include "programInput.h"
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <iostream>

int *pointLightOn, *dirLightOn, *ambientLightOn, *spotLightOn;

double _camZoom = 0;
double _xOffset, _yOffset, lastX, lastY;
float angleFlow = 0.02f;
float xAngle, yAngle;
float speed = 0.002f;

void referenceLights(int *ambientLight, int *dirLight, int *pointLight, int *spotLight)
{
	ambientLightOn = ambientLight;
	dirLightOn = dirLight;
	pointLightOn = pointLight;
	spotLightOn = spotLight;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (_camZoom < -40)
		_camZoom = -40;
	else if (_camZoom > 90)
		_camZoom = 90;
	if(_camZoom >= -40 && _camZoom <= 90)
		_camZoom -= yoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)//ambient light
		*ambientLightOn = (*ambientLightOn == 0) ? 1 : 0;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)//directional light
		*dirLightOn = (*dirLightOn == 0) ? 1 : 0;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)//point light
		*pointLightOn = (*pointLightOn == 0) ? 1 : 0;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)//spot light
		*spotLightOn = (*spotLightOn == 0) ? 1 : 0;

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
       glfwSetWindowShouldClose(window, 1);
}   

void update(GLFWwindow* window)
{
	lastX = _xOffset;
	lastY = _yOffset;
    glfwGetCursorPos(window, &_xOffset, &_yOffset);

	xAngle = (lastX - _xOffset) * speed;
	yAngle = (lastY - _yOffset) * speed;
}

void inputOnce(GLFWwindow* window)
{
    glfwSetScrollCallback(window, scroll_callback); // scroll wheel
    glfwSetKeyCallback(window, key_callback);		// keyboard
}

glm::mat4 updateZoom()
{
    glm::mat4 newProjection = glm::perspective(glm::radians(45.0f + (float)_camZoom), 16.0f/9.0f, 0.1f, 100.0f); 
    return newProjection;
}

void updateRotation(glm::mat4 &model, glm::mat4 &view)
{
	model *= glm::rotate(glm::mat4(), xAngle + angleFlow, glm::normalize(glm::vec3(0, 1.0, 0)));
	view *= glm::rotate(glm::mat4(),  yAngle, glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
}