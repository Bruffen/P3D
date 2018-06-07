#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "LoadOBJ.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

GLuint VAO, Buffers[3];
glm::mat4 Model, View, Projection;

void start(void);

int main(void)
{
	GLFWwindow *window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "Iron Man", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	start();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		/*
		static const GLfloat green[] = {
			0.1f, 0.6f, 0.0f, 0.0f
		};
		glClearBufferfv(GL_COLOR, 0, green);
		*/

		//display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void start(void)
{
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 5.0f),	// posicao
		glm::vec3(0.0f, 0.0f, 0.0f),	// lookat
		glm::vec3(0.0f, 1.0f, 0.0f)		// normal
	);

	//criar os arrays para guardar informação dentro da função load
	vector<glm::vec3> positions;
	vector<glm::vec2> texturecoordinates;
	vector<glm::vec3> normals;
	vector<unsigned int> positionIndices, textureCoordsIndices, normalIndices;
	//load do ficheiro obj para as variáveis
	Load("Iron_Man/Iron_Man.obj", positions, texturecoordinates, normals, positionIndices, textureCoordsIndices, normalIndices);

	//criar nome de VAO e vincular
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//criar nomes de VBOs
	glGenBuffers(3, Buffers);

	//primeiro VBO com as posicoes
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(positions), &positions[0], 0);
	//segundoVBO com as coordenadas de textura
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(texturecoordinates), &texturecoordinates[0], 0);
	//terceiro VBO com as normais
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(normals), &normals[0], 0);
}