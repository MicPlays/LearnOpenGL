#pragma once
#include "resourcemanager.h"
#include "cube.h"

class ColorDemo
{
	public:
		ColorDemo(const char* vpath, const char* fpath, ResourceManager* rm);
		Cube lightCube;
		Cube litCube;
		void Draw(Shader* shader, glm::mat4 proj, glm::mat4 view);
		Shader* lightShader;
		unsigned int VAO, VBO, VAO2;
};

