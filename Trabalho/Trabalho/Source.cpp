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
#include "LoadShaders.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

void start(void);
void draw(void);

GLuint VAO, Buffers[3], programa, numVertices;
glm::mat4 Model, View, Projection;
GLfloat angle = 0.0f;

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
		draw();

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
		glm::vec3(0.0f, 1.5f, 6.0f),	// posicao
		glm::vec3(0.0f, 1.5f, 0.0f),	// lookat
		glm::vec3(0.0f, 1.0f, 0.0f)		// normal
	);

	//criar os arrays para guardar informação dentro da função load
	vector<glm::vec3> positions;
	vector<glm::vec2> texturecoordinates;
	vector<glm::vec3> normals;

	//load do ficheiro obj para as variáveis
	Load("Iron_Man/Iron_Man.obj", positions, texturecoordinates, normals);
	numVertices = positions.size();

	//criar nome de VAO e vincular
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//criar nomes de VBOs
	glGenBuffers(3, Buffers);

	//primeiro VBO com as posicoes
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferStorage(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], 0);
	//segundoVBO com as coordenadas de textura
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferStorage(GL_ARRAY_BUFFER, texturecoordinates.size() * sizeof(glm::vec2), &texturecoordinates[0], 0);
	//terceiro VBO com as normais
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferStorage(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], 0);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER,   "shaders/shader.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/shader.frag" },
		{ GL_NONE, NULL }
	};

	programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	GLint positionsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition");
	glVertexAttribPointer(positionsId, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(positionsId);

	glViewport(0, 0, WIDTH, HEIGHT);

	//optimização
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Model = glm::rotate(glm::mat4(), angle += 0.02f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));

	GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));
	// Atribui valor ao uniform View
	GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
	// Atribui valor ao uniform Projection
	GLint projectionId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Projection");
	glProgramUniformMatrix4fv(programa, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}