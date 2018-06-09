#include "LoadOBJ.h"

void Load(string directory, string filename, vector<glm::vec3> &positions, vector<glm::vec2> &texturecoordinates, vector<glm::vec3> &normals, Material &material, string &textureName)
{
	FILE *file = fopen((directory + filename).c_str(), "r");

	if (file != NULL)
	{
		vector<glm::vec3> p;
		vector<glm::vec2> t;
		vector<glm::vec3> n;
		while (true)
		{
			char firstWord[64];
			int result = fscanf(file, "%s", firstWord);
			if (result == EOF)
				break;

			//posicao de vertice
			if (strcmp(firstWord, "v") == 0) {
				glm::vec3 position;
				fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
				//printf("v %f %f %f\n", position.x, position.y, position.z);
				p.push_back(position);
			}

			//coordenada textura
			else if (strcmp(firstWord, "vt") == 0) {
				glm::vec2 texturecoord;
				fscanf(file, "%f %f\n", &texturecoord.x, &texturecoord.y);
				//printf("vt %f %f\n", texturecoord.x, texturecoord.y);
				t.push_back(texturecoord);
			}

			//normal do vertice
			else if (strcmp(firstWord, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				//printf("vn %f %f %f\n", normal.x, normal.y, normal.z);
				n.push_back(normal);
			}

			//indices para a face do triangulo
			else if (strcmp(firstWord, "f") == 0) {
				unsigned int pIndex[3], tIndex[3], nIndex[3];
				fscanf(file, " %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&pIndex[0], &tIndex[0], &nIndex[0],		//primeiro vertice da face
					&pIndex[1], &tIndex[1], &nIndex[1],		//segundo vertice da face
					&pIndex[2], &tIndex[2], &nIndex[2]);	//terceiro vertice da face

				//formar array com os vértices totais a partir dos indices
				for (int i = 0; i < 3; i++)
				{
					positions.push_back(p[pIndex[i] - 1]);
					texturecoordinates.push_back(t[tIndex[i] - 1]);
					normals.push_back(n[nIndex[i] - 1]);
					//indice -1 porque no ficheiro obj, os indices começam no 1 e não em 0
				}
			}
			else if (strcmp(firstWord, "usemtl") == 0)
			{
				string materialName;
				fscanf(file, "%s\n", &materialName);
				strcat(&materialName[0], ".mtl");
				LoadMaterial(directory, materialName, material, textureName);
			}
		}
	}
}

void LoadMaterial(string directory, string filename, Material &material, string &textureName)
{
	FILE *file = fopen(strcat(&directory[0], &filename[0]), "r");

	if (file != NULL)
	{
		while (true)
		{
			char firstWord[64];
			int result = fscanf(file, "%s", firstWord);
			if (result == EOF)
				break;

			if (strcmp(firstWord, "Ns") == 0) {
				fscanf(file, "%f\n", &material.ns);
			}
			
			else if (strcmp(firstWord, "Ka") == 0) {
				fscanf(file, "%f %f %f\n", &material.ka.r, &material.ka.g, &material.ka.b);
			}

			else if (strcmp(firstWord, "Kd") == 0) {
				fscanf(file, "%f %f %f\n", &material.kd.r, &material.kd.g, &material.kd.b);
			}

			else if (strcmp(firstWord, "Ks") == 0) {
				fscanf(file, "%f %f %f\n", &material.ks.r, &material.ks.g, &material.ks.b);
			}

			else if (strcmp(firstWord, "Ni") == 0) {
				fscanf(file, "%f\n", &material.ni);
			}

			else if (strcmp(firstWord, "d") == 0) {
				fscanf(file, "%f\n", &material.d);
			}

			else if (strcmp(firstWord, "illum") == 0) {
				fscanf(file, "%f\n", &material.illum);
			}

			else if (strcmp(firstWord, "map_Kd") == 0) {
				fscanf(file, "%s\n", &textureName);
			}
		}
	}
}