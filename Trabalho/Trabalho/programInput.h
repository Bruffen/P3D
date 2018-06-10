#include <GLFW\glfw3.h>  
 
// when scroll wheel or touchpad gesture do this  
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);  
// update  
void update(GLFWwindow* window);  
// Things that are only needed to do once  
void inputOnce(GLFWwindow* window);
//updates the zoom
glm::mat4 updateZoom();

glm::mat4 updateRotation();