#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	const float cameraBaseSpeed = 10.0f;
	float cameraSpeed = 0.0f;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraYaw;
	glm::vec3 cameraUp;
	float lastX = 400.0f, lastY = 300.0f;
	float yaw = -90.0f, pitch = 0;
	const float sensitivity = 0.1f;
	float fov = 45.0f;

	enum Direction {forward, back, left, right};

	Camera();
	void processDirection(Direction directionInt);
	void processMouseMovement(float xpos, float ypos, bool firstMouse);
	void zoom(float yoffset);
};




