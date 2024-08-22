#pragma once
#include"shaderProgram/shader.h"
#include"camera/camera.h"
#include"entity/entity.h"



class SandBox {
private:
	unsigned int w_width, w_height;
	
	bool startRender;
	bool m_showGridMap;
	bool m_enableTileMapEditing;
	bool m_enableInterpolation;
	bool m_gravityInfluence;
	float initialZoom;
	float time;
	glm::vec2 m_mousePosScreen;
	glm::vec2 m_mousePosGlobal;


public:
	

	bool keys[1024];
	bool keyProcessed[1024];

	SandBox();
	~SandBox();

	void setCameraScroll(float yoffset);
	void GetMousePos(glm::vec2 pos);
	void updateMousePos();

	void m_movePlayer(glm::vec3 direction);
	void checkCollisionForEntity();
	void processInput(float dt);
	void renderGUI();
	void initRenderData();
	void update(float& dt);
	void updateVertices(Entity2D& entity,int entityIndex);
	void updateEntities(float& dt);
	void renderScene(float dt);
	void initSandBox(int width, int height);
	
};