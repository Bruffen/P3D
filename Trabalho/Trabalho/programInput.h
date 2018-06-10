#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

void referenceLights(int *ambientLight, int *dirLight, int *pointLight, int *spotLight);
// when scroll wheel or touchpad gesture do this  
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);  
// update  
void update(GLFWwindow* window);  
// Things that are only needed to do once  
void inputOnce(GLFWwindow* window);
//updates the zoom
glm::mat4 updateZoom();
//updates de rotation
glm::mat4 updateRotation(glm::mat4 matrix);