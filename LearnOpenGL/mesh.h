#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include "texture2d.h"
#include "shader.h"

class ResourceManager;

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture2D*> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D*> textures, ResourceManager* rm);
		void Draw(Shader* shader);

	private:
		unsigned int VAO, VBO, EBO;

		void setupMesh();
};

