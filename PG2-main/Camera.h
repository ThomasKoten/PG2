#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
class Camera
{
public:

	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;

	GLfloat Yaw = -90.0f;
	GLfloat Pitch = 0.0f;;

	// Camera options
	GLfloat MovementSpeed = 10.0f;
	GLfloat MouseSensitivity = 0.25f;

	Camera(glm::vec3 position);

	glm::mat4 GetViewMatrix();

	glm::vec3 ProcessInput(GLFWwindow* window, GLfloat deltaTime);

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch = GL_TRUE);

private:
	void updateCameraVectors();
};