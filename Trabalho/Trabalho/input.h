#include <GLFW\glfw3.h>  
 
//Variables  
double _camZoom = 0;  
double _xOffset, _yOffset;
  
// when scroll wheel or touchpad gesture do this  
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);  
// update  
void update(GLFWwindow* window);  
// Things that are only needed to do once  
void inputOnce(GLFWwindow* window);