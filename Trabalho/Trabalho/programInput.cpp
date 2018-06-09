#include  <GLFW\glfw3.h>  
 
double _camZoom = 0;
double _xOffset, _yOffset;

bool pointLightOn = false, dirLightOn = false, ambientLightOn = false, spotLightOn = false;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    _camZoom += xoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)//ambient light
        ambientLightOn = !ambientLightOn;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)//directional light
        dirLightOn = !dirLightOn;
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)//point light
        pointLightOn = !pointLightOn;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)//spot light
        spotLightOn = !spotLightOn;
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
    float xAngle, yAngle;
}

void inputOnce(GLFWwindow* window)
{
    glfwSetScrollCallback(window, scroll_callback); // scroll wheel
    glfwSetKeyCallback(window, key_callback); // keyboard
}