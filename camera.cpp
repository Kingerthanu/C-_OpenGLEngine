#include "camera.h"


Camera::Camera(int width, int height, glm::vec3 position)
{
	// Aspect ratio of screen of camera (eyes) and position of cam
	this->width = width;
	this->height = height;
	this->position = position;

}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{

	// Fill up a matrix with ones as our view as well as projection of view
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Set the view in the position of the eye and its sense of upward
	view = glm::lookAt(this->position, this->position + orientation, this->Up);

	// How the rays will be casted 
	projection = glm::perspective(glm::radians(FOVdeg), (float)(this->width / this->height), nearPlane, farPlane);

	// Our overall eyesight is based on the view we have and the projection we have of it
	this->camMatrix = projection * view;

}


void Camera::matrix(Shader& shader, const char* uniform)
{
	// Send through the cameras position to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(this->camMatrix));

}


