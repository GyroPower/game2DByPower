#pragma once
#include<glm/glm.hpp>

enum cameraMovement {
	UP,
	RIGHT,
	DOWN,
	LEFT
};



class Camera {

public:
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float Zoom;
	float movementSpeed;

	Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ);

	void ProcessKeyBoard(cameraMovement direction, float dt);
	void processMouseScroll(float yoffset);
	void moveCamera(glm::vec3 direction);

	glm::mat4 getViewMatrix();
	glm::mat4 getZoomMatrix();

private:

	void updateCameraVectors();
};