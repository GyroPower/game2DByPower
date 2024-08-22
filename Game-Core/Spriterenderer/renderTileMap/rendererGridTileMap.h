#pragma once
#include<vector>
#include"shaderProgram/shader.h"
#include"entity/entity.h"
#include"camera/camera.h"

// this class is for render a grid for a TileMap, this is I how would do this
// and i know there are better ways to do this, but this is my implementation of how i would do this

class RendererGridTileMap {
	Shader m_shader;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_VBO_Pos;
	GLuint m_IBO;
	GLuint m_indicesSize;
	int m_instances;
	glm::vec4 m_gridColor;
	bool m_initRender;

public:
	RendererGridTileMap(Shader& shader, glm::vec4 gridColor = glm::vec4(1.0f));
	~RendererGridTileMap();

	void initBufferData();
	void fillBufferData(float tileSize = 16.0f, float gridWidth = 200.0f, float gridHeight = 50.0f);
	void draw(Camera& camera);

};