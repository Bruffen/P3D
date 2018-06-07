#include "LoadOBJ.h"

void Load(string filename, vector<glm::vec3> &positions, vector<glm::vec2> &texturecoordinates, vector<glm::vec3> &normals, vector<unsigned int> &positionIndices, vector<unsigned int> &textureCoordsIndices, vector<unsigned int> &normalIndices)
{
	FILE *file = fopen(filename.c_str(), "r");

	if (file != NULL)
	{
		vector<glm::vec3> positions;
		vector<glm::vec2> texturecoordinates;
		vector<glm::vec3> normals;
		vector<unsigned int> positionIndices, textureCoordsIndices, normalIndices;

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
				positions.push_back(position);
			}

			//coordenada textura
			else if (strcmp(firstWord, "vt") == 0) {
				glm::vec2 texturecoord;
				fscanf(file, "%f %f\n", &texturecoord.x, &texturecoord.y);
				//printf("vt %f %f\n", texturecoord.x, texturecoord.y);
				texturecoordinates.push_back(texturecoord);
			}

			//normal do vertice
			else if (strcmp(firstWord, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				//printf("vn %f %f %f\n", normal.x, normal.y, normal.z);
				normals.push_back(normal);
			}
			
			//indices para a face do triangulo
			else if (strcmp(firstWord, "f") == 0) {
				unsigned int p[3], t[3], n[3];
				fscanf(file, " %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&p[0], &t[0], &n[0],	//primeiro vertice da face
					&p[1], &t[1], &n[1],	//segundo vertice da face
					&p[2], &t[2], &n[2]);	//terceiro vertice da face

				for (int i = 0; i < 3; i++)
				{
					positionIndices.push_back(p[i]);
					textureCoordsIndices.push_back(t[i]);
					normalIndices.push_back(n[i]);
				}
			}
		}
	}
}