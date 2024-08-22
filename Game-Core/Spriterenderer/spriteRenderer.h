#pragma once
#include"shaderProgram/shader.h"
#include<vector>
#include"entity/entity.h"
#include"camera/camera.h"

class SpriteRenderer {
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void reserveRenderBuffer();
	void draw(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
		Camera& camera, std::vector<Texture2D> textures);
	void fillData(std::vector<Vertex>& vertices, std::vector<unsigned int>& inidices);
	void updateData(std::vector<Vertex>& vertices, unsigned int entityIndex);

private:
	Shader shader;
	unsigned int VAO;
	unsigned int IBO;
	unsigned int VBO;
};

class SpriteRendererInstanced {
public:
	SpriteRendererInstanced(Shader& shader);
	~SpriteRendererInstanced();

	void reserveBuffer();
	void initFillData(std::vector<Entity2D_Instaciaded>& entities);
	void updateEntity(Entity2D_Instaciaded& entity);
	void addEntity(Entity2D_Instaciaded& entity);
	void emptyEntityData(Entity2D_Instaciaded& entity);
	void emptyAllData();
	void draw(Camera& camera,std::vector<Texture2D> textures,const char* rendererName);
	void scaleSingleEntity(Entity2D_Instaciaded& entity, float& scale, Camera& cam);

private:
	Shader shader;
	GLuint VAO;
	GLuint VBO;
	GLuint VBO_Color;
	GLuint VBO_Pos;
	GLuint VBO_Tex;
	GLuint IBO;
	GLuint VBO_Tex_Coords;

	int instances;
	
};