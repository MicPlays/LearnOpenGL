#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include "shader.h"
#include "camera.h"
#include "window.h"
#include "textrenderer.h"
#include "texture2d.h"
#include "resourcemanager.h"
#include "boxdemo.h"
#include "model.h"
#include "mesh.h"


GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM: { error = "INVALID_ENUM"; break; };
		case GL_INVALID_VALUE: { error = "INVALID_VALUE"; break; };
		case GL_INVALID_OPERATION: { error = "INVALID_OPERATION"; break; };
		case GL_OUT_OF_MEMORY: { error = "OUT_OF_MEMORY"; break; };
		case GL_INVALID_FRAMEBUFFER_OPERATION: { error = "INVALID_FRAMEBUFFER_OPERATION"; break; };
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError(glCheckError_)(__FILE__, __LINE__)

int main() {

	Window window = Window();
	if (window.glwindow == NULL) return -1;

	Camera camera;
	window.currentCamera = &camera;

	ResourceManager rm;

	//BoxDemo demo = BoxDemo(rm);

	Model* model = new Model("models/jugg/scene.gltf", "jugg", &rm);

	//shader for rendering text
	Shader* textShader;
	textShader = rm.load_shader("textshader.vs", "textshader.fs", "textShader");

	TextRenderer tr = TextRenderer("fonts/arial.ttf", &rm);

	Shader* ourShader = rm.load_shader("shader.vs", "shader.fs", "ourShader");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window.glwindow))
	{

		window.setDeltaTime();

		window.processInput();

		//clear color buffer with color specified by glClearColor()
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//demo.drawBoxes(camera);
		ourShader->use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)800.0f / (float)600.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
		ourShader->setMat4("proj", projection);
		ourShader->setMat4("view", view);

		glm::mat4 mod = glm::mat4(1.0f);
		mod = glm::translate(mod, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		mod = glm::scale(mod, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		ourShader->setMat4("model", mod);

		model->Draw(ourShader);
		glCheckError();
		//tr.RenderText(textShader, "Hello", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));

		//swaps color buffer for rendering output pixels
		glfwSwapBuffers(window.glwindow);
		//checks for events and updates window state accordingly
		glfwPollEvents();
	}

	//glfw cleanup
	delete tr.activeFont;
	delete model;
	//delete demo.tex1;
	//delete demo.tex2;
	glfwTerminate();
	return 0;
}

