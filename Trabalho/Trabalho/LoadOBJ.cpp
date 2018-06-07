#include "LoadOBJ.h"

void Load(string filename)
{
	FILE *file = fopen(filename.c_str(), "r");

	if (file != NULL)
	{
		vector<glm::vec3> positions;
		vector<glm::vec2> texturecoordinates;
		vector<glm::vec3> normals;
		vector<glm::vec3> positionIndices, textureCoordsIndices, normalIndices;
		unsigned int i = 0;

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
				positions.push_back(position);
			}

			//coordenada textura
			else if (strcmp(firstWord, "vt") == 0) {
				glm::vec2 texturecoord;
				fscanf(file, "%f %f\n", &texturecoord.x, &texturecoord.y);
				texturecoordinates.push_back(texturecoord);
			}

			//normal do vertice
			else if (strcmp(firstWord, "vn") == 0) {
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
			}

			//indices para a face do triangulo
			else if (strcmp(firstWord, "f") == 0) {
				fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&positionIndices[i + 0], &textureCoordsIndices[i + 0], &normalIndices[i + 0],	//primeiro vertice da face
					&positionIndices[i + 1], &textureCoordsIndices[i + 1], &normalIndices[i + 1],	//segundo vertice da face
					&positionIndices[i + 2], &textureCoordsIndices[i + 2], &normalIndices[i + 2]);	//terceiro vertice da face
			}
		}
	}
}