#include "colordemo.h"
#include "resourcemanager.h"
#include "cube.h"

ColorDemo::ColorDemo(const char* vpath, const char* fpath, ResourceManager* rm)
{
	this->lightShader = rm->load_shader(vpath, fpath, "colordemo");
	this->lightCube = Cube(&VAO, &VBO, glm::vec3(1.2f, 1.0f, 2.0f));
	this->litCube = Cube(&VAO2, &VBO, glm::vec3(0.0f, 0.0f, 0.0f));
}

void ColorDemo::Draw(Shader* shader, glm::mat4 proj, glm::mat4 view)
{
	lightShader->use();
	lightShader->setMat4("proj", proj);
	lightShader->setMat4("view", view);
	lightCube.Draw(lightShader, VAO);

	shader->use();
	shader->setMat4("proj", proj);
	shader->setMat4("view", view);
	shader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	litCube.Draw(shader, VAO2);
}