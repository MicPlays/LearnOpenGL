#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Camera::Camera()
{
	this->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::processDirection(Direction directionInt)
{
	if (directionInt == forward)
	{
		cameraPos += cameraSpeed * cameraYaw;
	}
	else if (directionInt == back)
	{
		cameraPos -= cameraSpeed * cameraYaw;
	}
	else if (directionInt == left)
	{
		cameraPos -= glm::normalize(glm::cross(cameraYaw, cameraUp)) * cameraSpeed;
	}
	else if (directionInt == right)
	{
		cameraPos += glm::normalize(glm::cross(cameraYaw, cameraUp)) * cameraSpeed;
	}
}

void Camera::processMouseMovement(float xpos, float ypos, bool firstMouse)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 cameraDirection;
	cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection.y = sin(glm::radians(pitch));
	cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(cameraDirection);

	glm::vec3 cameraRotation;
	cameraRotation.x = cos(glm::radians(yaw));
	cameraRotation.y = 0.0f;
	cameraRotation.z = sin(glm::radians(yaw));
	cameraYaw = glm::normalize(cameraRotation);
}

void Camera::zoom(float yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}