#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include "shader.h"
#include "resourcemanager.h"
#include "camera.h"
#include "texture2d.h"

class BoxDemo
{
	public:
		BoxDemo(ResourceManager rm);
		void drawBoxes(Camera camera);
		unsigned int VAO, VBO;
		float mixAmount = 0.2f;

		Texture2D* tex1;
		Texture2D* tex2;
		
		Shader* ourShader;
};

