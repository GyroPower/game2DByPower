#include "rendererDebugQuad.h"
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>

rendererDebugQuad::rendererDebugQuad(Shader& shader)
	:m_shader(shader)
{

	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glGenBuffers(1, &this->m_VBO_color);
	glGenBuffers(1, &this->m_VBO_pos);
}

rendererDebugQuad::~rendererDebugQuad() 
{}

void rendererDebugQuad::reserveData() {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	indices.reserve(5);
	vertices.reserve(4);

	vertices.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));
	vertices.emplace_back(glm::vec3(1.0f, 1.0f, 0.0f));
	vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f));
	vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));

	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(3);
	indices.emplace_back(3);
	indices.emplace_back(0);


	std::size_t vec4Size = sizeof(glm::vec4);
	glBindVertexArray(this->m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 1000, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribDivisor(3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 1000, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, vec4Size * 4, (void*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, vec4Size * 4, (void*)(vec4Size * 1));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, vec4Size * 4, (void*)(vec4Size * 2));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, vec4Size * 4, (void*)(vec4Size * 3));

	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	glGenBuffers(1, &this->m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	this->m_indices = indices.size();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void rendererDebugQuad::initData(std::vector<Entity2D_Instaciaded>& entities) {

	std::vector<glm::mat4> modelMat4;
	modelMat4.reserve(entities.size());

	std::vector<glm::vec4> colors;
	colors.reserve(entities.size());
	for (int i = 0; i < entities.size(); i++) {
		glm::mat4 model = glm::mat4(1.0f);
		Rect entityRect = entities[i].m_getEntityRect();
		model = glm::translate(model, glm::vec3(entityRect.pos,0.0f));
		model = glm::scale(model, glm::vec3(entityRect.size, 1.0f));
		modelMat4.emplace_back(model);
		colors.emplace_back(entities[i].m_color);
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_color);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(glm::vec4) * entities.size()), &colors[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_pos);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(glm::mat4) * entities.size()), &modelMat4[0]);

	this->m_instances = entities.size();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void rendererDebugQuad::draw(Camera& camera) {
#if DEBUG
	if (!m_initRender)
	{
		std::cout << "render debug quad\n";
		
		m_initRender = true;
	}
#endif
	this->m_shader.use();
	this->m_shader.setMat4("view", camera.getViewMatrix());
	this->m_shader.setMat4("modelZoomCamera", camera.getZoomMatrix());


	glBindVertexArray(this->m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_IBO);
	glDrawElementsInstanced(GL_LINES, this->m_indices, GL_UNSIGNED_INT, 0, this->m_instances);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void rendererDebugQuad::updateData(Entity2D_Instaciaded& entity) 
{
	glm::mat4 model(1.0f);
	Rect entityRect = entity.m_getEntityRect();
	model = glm::translate(model, glm::vec3(entityRect.pos,0.0f));
	model = glm::scale(model, glm::vec3(entityRect.size, 1.0f));
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_pos);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(glm::mat4) * entity.m_returnRenderIndex()), sizeof(glm::mat4), &model);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO_color);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(glm::vec4) * entity.m_returnRenderIndex()), sizeof(glm::vec4), &entity.m_color);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}