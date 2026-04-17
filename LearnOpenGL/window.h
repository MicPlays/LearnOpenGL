#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera;

class Window
{
	public:
		Window() { InitWindow(); };
		void InitWindow();

		void processInput();
		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		void setDeltaTime();

		Camera* currentCamera;
		bool firstMouse = true;
		GLFWwindow* glwindow;
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
};


