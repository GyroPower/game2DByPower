#pragma once
#include"shaderProgram/shader.h"
#include<vector>
#include"entity/entity.h"
#include"camera/camera.h"

/*
	this spriteRenderer class is for use batchRendering but I stoped to develop to use 
	instance rendering
*/
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
	bool initRender = false;
};

/*
	This spriteRenderer class is for using instance rendering with OpenGL;
	I wiil in the future change to be able to use OpengGL or Vulkan but that will be later,

	For those who doesn't know(mean beginers in programing and in c++),
	to handle polymorfism in vectors you would probably use a vector pointer of the base
	class to be able to use the virtual functions of the base class redefined in the children classes,
	that's why I use a vector pointer reference in the initFillData function 
	(std::vector<baseClass*>& vectorName) 

	*/

class SpriteRendererInstanced {
public:
	SpriteRendererInstanced(Shader& shader, std::string rendererName);
	~SpriteRendererInstanced();

	void reserveBuffer(int sizeToReserve = 1000);
	void initFillData(std::vector<Entity2D_Instaciaded*>& entities);
	void updateEntity(Entity2D_Instaciaded& entity);
	void addEntity(Entity2D_Instaciaded& entity);
	void emptyEntityData(Entity2D_Instaciaded& entity);
	void emptyAllData();
	void draw(Camera& camera,std::vector<Texture2D>& textures);
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
	std::string renderName;

	int instances;
	int m_sizeReserved = 0;
	bool m_initRender = false;
};