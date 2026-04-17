#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include "texture2d.h"
#include "resourcemanager.h"


class Cube
{
	public:
		Cube() {};
		Cube(unsigned int *VAO, unsigned int *VBO, glm::vec3 pos);
		virtual void InitCube(unsigned int *VAO, unsigned int *VBO);
		virtual void Draw(Shader* shader, unsigned int VAO);
		glm::vec3 pos;
};

