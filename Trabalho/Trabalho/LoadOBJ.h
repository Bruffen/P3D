#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <glm\glm.hpp>

using namespace std;

struct Material
{
	//cor ambiente
	glm::vec3 ka;
	//cor difusa
	glm::vec3 kd;
	//cor especular
	glm::vec3 ks;
	//expoente especular
	float ns;
	//índice de refração da luz
	float ni;
	//dissolvência (oposto de transparência)
	float d;
	//tipo de modelo a utilizar
	unsigned short illum;

	//0		Color on and Ambient off
	//1		Color on and Ambient on
	//2		Highlight on
	//3		Reflection on and Ray trace on
	//4		Transparency: Glass on
	//		Reflection : Ray trace on
	//5		Reflection : Fresnel on and Ray trace on
	//6		Transparency : Refraction on
	//		Reflection : Fresnel off and Ray trace on
	//7		Transparency : Refraction on
	//		Reflection : Fresnel on and Ray trace on
	//8		Reflection on and Ray trace off
	//9		Transparency : Glass on
	//		Reflection : Ray trace off
	//10	Casts shadows onto invisible surfaces
};

void Load(string directory, string filename, vector<glm::vec3> &positions, vector<glm::vec2> &texturecoordinates, vector<glm::vec3> &normals, Material &material, string &textureName);
void LoadMaterial(string directory, string filename, Material &material, string &textureName);
