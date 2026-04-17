#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"

void Window::processInput()
{
	currentCamera->cameraSpeed = currentCamera->cameraBaseSpeed * deltaTime;
	//if ESC pressed, close window
	if (glfwGetKey(glwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glwindow, true);

	if (glfwGetKey(glwindow, GLFW_KEY_W) == GLFW_PRESS)
		currentCamera->processDirection(currentCamera->forward);
	if (glfwGetKey(glwindow, GLFW_KEY_S) == GLFW_PRESS)
		currentCamera->processDirection(currentCamera->back);
	if (glfwGetKey(glwindow, GLFW_KEY_A) == GLFW_PRESS)
		currentCamera->processDirection(currentCamera->left);
	if (glfwGetKey(glwindow, GLFW_KEY_D) == GLFW_PRESS)
		currentCamera->processDirection(currentCamera->right);

	/*
	if (glfwGetKey(glwindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		*mixAmount += 0.001f;
		if (*mixAmount >= 1.0f)
			*mixAmount = 1.0f;
	}

	if (glfwGetKey(glwindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		*mixAmount -= 0.001f;
		if (*mixAmount <= 0.0f)
			*mixAmount = 0.0f;
	}
	*/
}

void Window::InitWindow()
{
	//init GLFW (setting OpenGL version and Core Profile to configure OpenGL context)
	//GLFW handles the OpenGL context and program window stuff
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//init window
	this->glwindow = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (glwindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	//set window's context (main context)
	glfwMakeContextCurrent(glwindow);

	//init GLAD (pass the function to load the address of the OpenGL function pointers)
	//remember GLAD handles all of the OS-specific boilerplate code required to run OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	//since GLFW is a C library, it doesn't allow for class methods to be set as the callback. to get around this,
	//we utilize GLFW's user pointer to get a function pointer and set the callback
	glfwSetWindowUserPointer(this->glwindow, this);

	auto func = [](GLFWwindow* w, double xpos, double ypos)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(w))->mouse_callback(w, xpos, ypos);
	};

	glfwSetCursorPosCallback(glwindow, func);

	auto func2 = [](GLFWwindow* w, double xoffset, double yoffset)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(w))->scroll_callback(w, xoffset, yoffset);
	};

	glfwSetScrollCallback(glwindow, func2);

	auto func3 = [](GLFWwindow* w, int width, int height)
	{
		static_cast<Window*>(glfwGetWindowUserPointer(w))->scroll_callback(w, width, height);
	};

	//register window resize callback
	glfwSetFramebufferSizeCallback(glwindow, func3);

	//set viewport dimensions
	//first 2 params = lower left corner pos, 3rd + 4th = width + height
	glViewport(0, 0, 800, 600);
	glfwSetInputMode(glwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	currentCamera->processMouseMovement(xpos, ypos, firstMouse);
	if (firstMouse) firstMouse = false;
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	currentCamera->zoom((float)yoffset);
}

void Window::setDeltaTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}