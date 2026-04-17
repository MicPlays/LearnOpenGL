#include "boxdemo.h"
#include <glad/glad.h>
#include "shader.h"
#include "resourcemanager.h"
#include "camera.h"
#include "texture2d.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <GLFW/glfw3.h>

BoxDemo::BoxDemo(ResourceManager rm)
{
	this->ourShader = rm.load_shader("shader.vs", "shader.fs", "ourShader");
	//create textures
	tex1 = rm.load_texture("container.jpg", false, false, "container");
	tex2 = rm.load_texture("awesomeface.png", true, false, "awesomeface");

}

void BoxDemo::drawBoxes(Camera camera)
{
	ourShader->use();

	glActiveTexture(GL_TEXTURE0);
	tex1->Bind();
	glActiveTexture(GL_TEXTURE1);
	tex2->Bind();

	ourShader->setInt("texture1", 0);
	ourShader->setInt("texture2", 1);

	ourShader->setFloat("mixAmount", mixAmount);
	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < 10; i++)
	{

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 0;
		if (i % 3 == 0)
		{
			angle = 20.0f * (i + 2.0f);
		}
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader->setMat4("model", model);

		/*
		glm::mat4 view;
		view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
		glm::vec3 camRight = glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp));
		glm::vec3 camUp = glm::normalize(glm::cross(camRight, camera.cameraFront));
		glm::mat4 rot(1.0f);
		rot[0] = glm::vec4(camRight, 0);
		rot[1] = glm::vec4(camUp, 0);
		rot[2] = glm::vec4(-camera.cameraFront, 0);
		rot = glm::inverse(rot);
		glm::mat4 trans(1.0f);
		trans = glm::inverse(glm::translate(trans, camera.cameraPos));
		view = rot * trans;

		glm::mat4 proj;
		proj = glm::perspective(glm::radians(45.0fwindow.currentCamera->fov), 800.0f / 600.0f, 0.1f, 100.0f);

		unsigned int viewLoc = glGetUniformLocation(*ourShader->ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		unsigned int projLoc = glGetUniformLocation(*ourShader->ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		*/
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}