#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#define _CRT_SECURE_NO_WARNINGS

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

#include "programInput.h"

#define WIDTH 1280
#define HEIGHT 720

void start(void);
void draw(void);
void load_texture(string directory, string filename);

GLuint VAO, Buffers[3], programa, numVertices;
glm::mat4 Model, View, Projection;
glm::mat3 NormalMatrix;
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
	inputOnce(window);
	while (!glfwWindowShouldClose(window)) {
		draw();
		update(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void start(void)
{
	Projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	View = glm::lookAt(
		glm::vec3(0.0f, 1.5f, 6.0f),	// posicao
		glm::vec3(0.0f, 1.5f, 0.0f),	// lookat
		glm::vec3(0.0f, 1.0f, 0.0f)		// normal
	);

	//criar os arrays para guardar informa��o dentro da fun��o load
	vector<glm::vec3> positions;
	vector<glm::vec2> texturecoordinates;
	vector<glm::vec3> normals;
	Material material;
	string directory = "Iron_Man/";
	string textureName;

	//load do ficheiro obj para as vari�veis
	Load(directory, "Iron_Man.obj", positions, texturecoordinates, normals, material, textureName);
	numVertices = positions.size();
	load_texture(directory, textureName);

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
		{ GL_VERTEX_SHADER, "shaders/shader.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/shader.frag" },
		{ GL_NONE, NULL }
	};

	programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	//dar ao shader as informações
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	GLint positionsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition");
	glVertexAttribPointer(positionsId, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionsId);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	GLint textureCoordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vTextureCoords");
	glVertexAttribPointer(textureCoordsId, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureCoordsId);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	GLint normalId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vNormal");
	glVertexAttribPointer(normalId, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalId);

	GLint textureId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Texture");
	glProgramUniform1i(programa, textureId, 0 /* Unidade de Textura #0 */);

	// Fonte de luz ambiente global
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));

	// Fonte de luz direcional
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

	// Fonte de luz pontual
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight.position"), 1, glm::value_ptr(glm::vec3(-2.0, 2.0, 5.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight.constant"), 1.0f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight.linear"), 0.06f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight.quadratic"), 0.02f);

	// Fonte de Luz conica
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.position"), 1, glm::value_ptr(glm::vec3(-2.0, 2.0, 5.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.constant"), 1.0f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.linear"), 0.06f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.quadratic"), 0.02f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.spotCutoff"), 0.02f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.spotExponent"), 0.06f);
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "spotLight.spotDirection"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));

	// Propriedades do material
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.emissive"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.ambient"), 1, glm::value_ptr(material.ka));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.diffuse"), 1, glm::value_ptr(material.kd));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.specular"), 1, glm::value_ptr(material.ks));
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.shininess"), material.ns);
	// Material não tem cor ambiente nem especular (???)

	/*
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.emissive"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.shininess"), 12.0f);
	*/

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
	glm::mat4 ModelView = View * Model;
	NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));

	// Atribui valor ao uniform Model
	/*GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));*/
	// Atribui valor ao uniform View
	GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
	// Atribui valor ao uniform Projection
	GLint projectionId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Projection");
	glProgramUniformMatrix4fv(programa, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));
	// Atribui valor ao uniform ModelView
	GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
	// Atribui valor ao uniform NormalMatrix
	GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void load_texture(string directory, string filename)
{
	GLuint texture = 0;
	glGenBuffers(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nChannels;
	unsigned char *imageData = stbi_load(strcat(&directory[0], &filename[0]), &width, &height, &nChannels, 0);
	if (imageData) {
		// Carrega os dados da imagem para o Objeto de Textura vinculado ao target GL_TEXTURE_2D da Unidade de Textura ativa.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Liberta a imagem da memória do CPU
		stbi_image_free(imageData);
	}
	else {
		cout << "Error loading texture!" << endl;
	}
}