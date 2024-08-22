#pragma once
#include "shaderProgram/shader.h"
#include "entity/entity.h"
#include <vector>
#include "camera/camera.h"

class rendererDebugQuad {

public:
	rendererDebugQuad(Shader& shader);
	~rendererDebugQuad();

	void reserveData();
	void initData(std::vector<Entity2D_Instaciaded>& entities);
	void updateData(Entity2D_Instaciaded& entity);
	void draw(Camera& camera);

private:
	Shader m_shader;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_VBO_pos;
	GLuint m_VBO_color;
	GLuint m_IBO;

	int m_instances = 0;
	bool m_initRender = false; 
	GLuint m_indices = 0;

};