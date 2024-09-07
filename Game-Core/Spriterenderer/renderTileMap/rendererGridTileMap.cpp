#include "rendererGridTileMap.h"
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>

RendererGridTileMap::RendererGridTileMap(Shader& shader, glm::vec4 gridColor) 
	: m_shader(shader), m_gridColor(gridColor),m_instances(0),m_indicesSize(0),m_initRender(false)
{
	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glGenBuffers(1, &this->m_VBO_Pos);
}

RendererGridTileMap::~RendererGridTileMap() { }

void RendererGridTileMap::initBufferData() 
{
	float colorX = this->m_gridColor.x;
	float colorY = this->m_gridColor.y;
	float colorZ = this->m_gridColor.z;
	float colorA = this->m_gridColor.w;

	float vertices[] = {
		0.0f, 0.0f, 0.0f, colorX, colorY, colorZ, colorA,
		1.0f, 0.0f, 0.0f, colorX, colorY, colorZ, colorA,
		1.0f, 1.0f, 0.0f, colorX, colorY, colorZ, colorA,
		0.0f, 1.0f, 0.0f, colorX, colorY, colorZ, colorA
	};

	GLuint indices[] = {
		0,1, 1,2, 2,3, 3,0
	};

	std::size_t vec4Size = sizeof(glm::vec4);

	glBindVertexArray(this->m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(sizeof(float) * 3));
	

	int size = 1000 * 100;
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_Pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * size, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vec4Size * 4, (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vec4Size * 4, (void*)(vec4Size * 1));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, vec4Size * 4, (void*)(vec4Size * 2));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, vec4Size * 4, (void*)(vec4Size * 3));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glGenBuffers(1, &this->m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	this->m_indicesSize = 8;

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RendererGridTileMap::fillBufferData(float tileSize, float gridWidth, float gridHeight)
{
	std::vector<glm::mat4> positions;
	positions.reserve(gridWidth * gridHeight);

	
	for (int y = 0; y < gridHeight; y++) {
		for (int x = 0; x < gridWidth; x++) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(x * tileSize, y * tileSize, 0.0f));
			model = glm::scale(model, glm::vec3(tileSize, tileSize, 1.0f));
			positions.emplace_back(model);
		}
	}

	glBindVertexArray(this->m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_Pos);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * positions.size(), &positions[0]);

	this->m_instances = positions.size();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void RendererGridTileMap::draw(Camera& camera) 
{
#if DEBUG
	if (!this->m_initRender) {
		std::cout<< "Draw call " << "render grid\n";
		this->m_initRender = true;
	}
#endif
	this->m_shader.use();
	this->m_shader.setMat4("view", camera.getViewMatrix());

	glBindVertexArray(this->m_VAO);
	glDrawElementsInstanced(GL_LINES, 8, GL_UNSIGNED_INT, 0, this->m_instances);

	glBindVertexArray(0);

}