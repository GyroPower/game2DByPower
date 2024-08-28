#include"spriteRenderer.h"
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include"debugLog/debugLog.h"


SpriteRenderer::SpriteRenderer(Shader& shader) {
	this->shader = shader;
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->IBO);

}

SpriteRenderer::~SpriteRenderer(){ 
	glDeleteVertexArrays(1, &this->VAO);
}

void SpriteRenderer::reserveRenderBuffer() {
	glBindVertexArray(this->VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, 1000 * (4 * sizeof(Vertex)), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texSlot));

	glGenBuffers(1, &this->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(unsigned int) * 6) * 1000, nullptr, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void SpriteRenderer::fillData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), &vertices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * indices.size(),&indices[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SpriteRenderer::updateData(std::vector<Vertex>& vertices,unsigned int entityIndex) {
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(Vertex) * 4) * entityIndex, sizeof(Vertex) * vertices.size(),
		&vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderer::draw(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
	Camera& camera,std::vector<Texture2D> textures) {
		
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(glm::vec2(camera.Zoom), 1.0f));

	this->shader.use();
	this->shader.setMat4("view", camera.getViewMatrix());
	this->shader.setMat4("model", model);
	
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i].bind();
		
	}

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

///////////////////////////////////
// SpriRendererInstancing 
//////////////////////////////////

SpriteRendererInstanced::SpriteRendererInstanced(Shader& shader,std::string rendererName)
	:renderName(rendererName)
{
	this->shader = shader;
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->VBO_Pos);
	glGenBuffers(1, &this->VBO_Tex);
	glGenBuffers(1, &this->VBO_Tex_Coords);
	glGenBuffers(1, &this->VBO_Color);
	
	this->instances = 0;
}

SpriteRendererInstanced::~SpriteRendererInstanced(){ }

void SpriteRendererInstanced::reserveBuffer(int sizeToReserve) {

	this->m_sizeReserved = sizeToReserve;
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::mat4> mat4Pos;
	indices.reserve(6);
	vertices.reserve(4);
	mat4Pos.reserve(3);
	glm::vec4 colorS(1.0f, 1.0f, 0.4f, 1.0f);
	float texSlotS = -1.0f;


	vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
	vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f));
	vertices.emplace_back(glm::vec3(1.0f, 1.0f, 0.0f));
	vertices.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));

	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(0);
	indices.emplace_back(3);
	indices.emplace_back(2);

	
	std::size_t vec4Size = sizeof(glm::vec4);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex_Coords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords) * this->m_sizeReserved, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TexCoords), (void*)0);
	glVertexAttribDivisor(1, 1);

	

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->m_sizeReserved, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->m_sizeReserved, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribDivisor(3, 1);


	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->m_sizeReserved, nullptr, GL_DYNAMIC_DRAW);
	
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

	glGenBuffers(1, &this->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// filling data to the renderer buffer
void SpriteRendererInstanced::initFillData(std::vector<Entity2D_Instaciaded>& entities) {

	std::vector<glm::mat4> modelMat4;
	modelMat4.reserve(entities.size());

	std::vector<float> texSlots;
	texSlots.reserve(entities.size());

	std::vector<glm::vec4> colors;
	colors.reserve(entities.size());

	std::vector<TexCoords> texCoords;
	texCoords.reserve(entities.size());

	for (int i = 0; i < entities.size(); i++) {
		
		Entity2D_Instaciaded& entity = entities[i];
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, entities[i].m_position);
		model = glm::scale(model, glm::vec3(entities[i].m_size, 1.0f));
		modelMat4.emplace_back(model);
		texSlots.emplace_back(entities[i].texSlot);
		colors.emplace_back(entities[i].m_color);
		texCoords.emplace_back(entities[i].m_texCoords);

		
		
	}

	
	glBindVertexArray(this->VAO);

	//this is a poor fix just to add data after the player data was added to the renderer
	int offsetColor = this->instances == 1 ? sizeof(glm::vec4) : 0;
	int offsetTex = this->instances == 1 ? sizeof(float): 0;
	int offsetPos = this->instances == 1 ? sizeof(glm::mat4) : 0;
	int offsetTexCoords = this->instances == 1 ? sizeof(TexCoords) : 0;

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Color);
	glBufferSubData(GL_ARRAY_BUFFER, offsetColor, (sizeof(glm::vec4) * entities.size()), &colors[0]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex);
	glBufferSubData(GL_ARRAY_BUFFER, offsetTex, (sizeof(float) * entities.size()), &texSlots[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Pos);
	glBufferSubData(GL_ARRAY_BUFFER, offsetPos, (sizeof(glm::mat4) * entities.size()), &modelMat4[0]);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex_Coords);
	glBufferSubData(GL_ARRAY_BUFFER, offsetTexCoords, sizeof(TexCoords) * entities.size(), &texCoords[0]);

	//here just check if the player was added, meaning that it only have one instance
	if (this->instances == 1)
		this->instances += entities.size();
	else 
		this->instances = entities.size();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
}

void SpriteRendererInstanced::updateEntity(Entity2D_Instaciaded& entity) {
	this->renderName;
	glm::mat4 model(1.0f);
	model = glm::translate(model, entity.m_position);
	model = glm::scale(model, glm::vec3(entity.m_size, 1.0f));

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * entity.m_returnRenderIndex(), sizeof(glm::mat4), &model);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex_Coords);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(TexCoords) * entity.m_returnRenderIndex(), sizeof(TexCoords), &entity.m_texCoords);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Color);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * entity.m_returnRenderIndex(), sizeof(glm::vec4), &entity.m_color);

	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRendererInstanced::addEntity(Entity2D_Instaciaded& entity)
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, entity.m_position);
	model = glm::scale(model, glm::vec3(entity.m_size, 1.0f));
	
	glBindVertexArray(this->VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Color);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * entity.m_returnRenderIndex(), sizeof(glm::vec4), &entity.m_color);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * entity.m_returnRenderIndex(), sizeof(float), &entity.texSlot);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * entity.m_returnRenderIndex(), sizeof(glm::mat4), &model);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex_Coords);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(TexCoords) * entity.m_returnRenderIndex(), sizeof(TexCoords), &entity.m_texCoords);


	this->instances +=1;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRendererInstanced::emptyEntityData(Entity2D_Instaciaded& entity)
{
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Color);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * entity.m_returnRenderIndex(), sizeof(glm::vec4), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * entity.m_returnRenderIndex(), sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * entity.m_returnRenderIndex(), sizeof(glm::mat4), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex_Coords);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(TexCoords) * entity.m_returnRenderIndex(), sizeof(TexCoords), nullptr);


	this->instances -= 1;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRendererInstanced::emptyAllData()
{
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Color);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * this->m_sizeReserved, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * this->m_sizeReserved, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Pos);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * this->m_sizeReserved, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Tex_Coords);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TexCoords) * this->m_sizeReserved, nullptr);


	this->instances = 0;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRendererInstanced::draw(Camera& camera, std::vector<Texture2D> textures) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(camera.Zoom, camera.Zoom, 1.0f));
	model = glm::scale(model, glm::vec3(camera.Zoom, camera.Zoom, 1.0f));
	
	// log in console the renderer draw call
#if DEBUG
	if (!m_initRender)
	{
		std::cout << this->renderName <<"\n";
		m_initRender = true;
	}
#endif
	this->shader.use();
	this->shader.setMat4("view", camera.getViewMatrix());
	this->shader.setMat4("modelZoomCamera", camera.getZoomMatrix());
	
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i].bind();
	}

	glBindVertexArray(this->VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, this->instances);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SpriteRendererInstanced::scaleSingleEntity(Entity2D_Instaciaded& entity, float& scale, Camera& cam) {

	glBindVertexArray(this->VAO);
	glm::mat4 model(1.0f);
	model = glm::translate(model, entity.m_position);
	model = glm::scale(model, glm::vec3(entity.m_size + scale , 1.0f));
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_Pos);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(glm::mat4) * entity.m_entityIndex), sizeof(glm::mat4), &model);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}