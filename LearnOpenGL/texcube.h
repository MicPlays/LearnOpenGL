#pragma once

#include "cube.h"
#include <vector>

class TexCube : public Cube
{
	public:
		TexCube(unsigned int* VAO, unsigned int* VBO, char* path, bool alpha, char* name, ResourceManager* rm, glm::vec3 pos);
		~TexCube();
		glm::vec3 pos;
		std::vector<Texture2D*> textures;
		void AddTexture(char* path, bool alpha, char* name, ResourceManager* rm);
		void InitCube(unsigned int* VAO, unsigned int* VBO) override;
		void Draw(Shader* shader, unsigned int VAO) override;
		
};

