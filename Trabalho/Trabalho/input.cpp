#include "input.h"

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    _camZoom += xoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) ;//ambient light

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) ;//directional light

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) ;//point light

    if (key == GLFW_KEY_4 && action == GLFW_PRESS) ;//spot light
}

void update(GLFWwindow* window)
{
    glfwGetCursorPos(window, &_xOffset, &_yOffset);

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    double cx = w/2, cy = h/2;

    //changed the coordinates so middle is 0,0
    double _newX = _xOffset - cx, _newY = _yOffset - cy;

    //create the matrix for the rotation
    //angle change rate
}

void inputOnce(GLFWwindow* window)
{
    glfwSetScrollCallback(window, scroll_callback); // scroll wheel
    glfwSetKeyCallback(window, key_callback); // keyboard
}