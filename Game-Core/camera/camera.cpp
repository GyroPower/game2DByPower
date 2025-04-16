#include"camera.h"

#include<glm/gtc/matrix_transform.hpp>


const float SPEED = 120.5f;
const float ZOOM = 1.0f;

Camera::Camera(glm::vec2 projectionSize, glm::vec3 position, glm::vec3 up) 
	:pos(position),up(up),worldUp(0.0f,1.0f,0.0f),front(0.0f,0.0f,-1.0f), movementSpeed(SPEED), 
	Zoom(ZOOM), right(1.0f,0.0,0.0f), m_projectionSize(projectionSize)
{ }

Camera::Camera(float projectSizeX, float projectSizeY, float posX, float posY, float posZ, float upX, float upY, float upZ)
	:pos(posX,posY,posZ), up(upX,upY,upZ), worldUp(0.0f,1.0f,0.0f), front(0.0f,0.0f,-1.0f),
	movementSpeed(SPEED), Zoom(ZOOM),m_projectionSize(projectSizeX, projectSizeY)
{ }

void Camera::ProcessKeyBoard(cameraMovement direction, float dt) {
	float velocity = this->movementSpeed * dt;

	if (direction == UP)
		this->pos -= this->up * velocity;
	if (direction == DOWN)
		this->pos += this->up * velocity;
	if (direction == RIGHT)
		this->pos += this->right * velocity;
	if (direction == LEFT)
		this->pos -= this->right * velocity;

}

void Camera::processMouseScroll(float yoffset) {
	this->Zoom += (float)yoffset / 20.0f;

	if(this->Zoom < 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom > 5.0f)
		this->Zoom = 5.0f;
}

glm::mat4 Camera::getViewMatrix() {
	
	glm::mat4 lookMatrix = glm::lookAt(this->pos, this->pos + this->front, this->up);

	lookMatrix = glm::scale(lookMatrix, glm::vec3(this->Zoom, this->Zoom, 1.0f));

	return lookMatrix;
}

glm::mat4 Camera::getZoomMatrix() {
	glm::mat4 zoomMatrix(1.0f);
	//zoomMatrix = glm::translate(zoomMatrix, glm::vec3(this->Zoom, this->Zoom , 1.0f));
	zoomMatrix = glm::scale(zoomMatrix, glm::vec3(this->Zoom, this->Zoom, 1.0f));
	return zoomMatrix;
}

glm::vec3 Camera::getCameraPos()
{
	return this->pos;
}

glm::vec2 Camera::getProjectionSize()
{
	return glm::vec2(this->m_projectionSize.x / this->Zoom, this->m_projectionSize.y / this->Zoom);
}

void Camera::updateCameraVectors() {}
