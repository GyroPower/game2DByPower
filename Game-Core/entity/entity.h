#pragma once
#include<glm/glm.hpp>
#include<vector>
#include"Texture/texture.h"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
	float texSlot;
	glm::vec4 color;

	Vertex(glm::vec3 position, glm::vec2 texCoords, glm::vec4 color, float texSlot)
		: position(position), texCoords(texCoords), color(color), texSlot(texSlot)
	{ }

	Vertex(glm::vec3 position, glm::vec2 texCoords, float texSlot)
		:position(position), texCoords(texCoords), color(glm::vec4(1.0f)), texSlot(texSlot)
	{ }


};

//This struct will help for simple 2d collision and reposition of the entites
struct Rect {
	//offsets for resizable
	glm::vec2 posOffset;
	glm::vec2 sizeOffset;

	glm::vec2 previusPos;
	glm::vec2 pos;
	glm::vec2 size;

	Rect(glm::vec2 _pos = glm::vec2(0.0f), glm::vec2 _size = glm::vec2(0.0f),
		glm::vec2 _prevPos = glm::vec2(0.0f), glm::vec2 _posOffset = glm::vec2(0.0f),
		glm::vec2 _sizeOffset = glm::vec2(0.0f))
		:pos(_pos), size(_size), previusPos(_prevPos), posOffset(_posOffset), sizeOffset(_sizeOffset)
	{
		if (this->posOffset.x != 0.0f || this->posOffset.y != 0.0f)
		{
			this->previusPos += this->posOffset; 
			this->pos += this->posOffset; 
		}
		if (this->sizeOffset.x != 0.0f || this->sizeOffset.y != 0.0f)
		{
			this->size += this->sizeOffset; 
		}
	}
	~Rect() {}
};

struct TexCoords {
	glm::vec2 position;
	glm::vec2 size;

	TexCoords(glm::vec2 position, glm::vec2 size)
		:position(position), size(size)
	{ }

	TexCoords()
		:position(glm::vec2(0.0f)), size(glm::vec2(16.0f))
	{ }
};

enum EntityState {
	IDLE,
	MOVE,
	JUMP,
	FALL,
};

class Entity2D {

public:
	float texSlot;
	glm::vec3 position;
	glm::vec2 size;
	glm::vec4 color;
	glm::vec2 maxTexCoords; // this how big are x and y axis, meaning width and height from a 0.0(bottom left) origin
	std::vector<Vertex> vertexs;
	std::vector<unsigned int> indices;
	

	Entity2D(float texSlot = -1.0f, glm::vec4 color = glm::vec4(1.0f), glm::vec3 position = glm::vec3(0.0f),
		glm::vec2 size = glm::vec2(10.0f));
	
	void updateVertexs();
	void updateTopRightTexCoords();
};



class Entity2D_Instaciaded {

public:
	float texSlot;
	float m_maxSpeed;
	glm::vec3 m_speed;
	glm::vec3 m_position;
	glm::vec3 m_previusPos;
	glm::vec3 direction;
	glm::vec2 m_size;
	glm::vec4 m_color;
	TexCoords m_texCoords;
	glm::vec2 m_texOffset;
	glm::vec2 m_posOffsetRect;
	glm::vec2 m_sizeOffsetRect;
	int m_entityIndex;
	int m_renderInstanceIndex;
	EntityState state;
	
	bool m_grounded;
	bool m_wallTouch;
	bool m_pushed;
	bool m_right;
	bool m_left;

	Entity2D_Instaciaded(int* renderIndex = nullptr, glm::vec3 position = glm::vec3(0.0f), glm::vec2 size = glm::vec2(10.0f),
		glm::vec4 color = glm::vec4(1.0f), glm::vec2 posOffsetRect = glm::vec2(0.0f), glm::vec2 sizeOffsetRect = glm::vec2(0.0f), float texSlot = -1.0f, glm::vec2 texPos = glm::vec2(0.0f),
		glm::vec2 texSize = glm::vec2(16.0f));

	~Entity2D_Instaciaded();

	void setNewTexOffset(glm::vec2 newOffset);
	int m_returnRenderIndex();
	void move(float& dt);
	void setPosInterpolation(float& dt);
	
	virtual Rect m_getEntityRect();
};

class Tile : public Entity2D_Instaciaded {
	
	bool m_visible;
	

public:
	Tile(int* renderIndex,glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2 size = glm::vec2(16.0f),
		glm::vec4 color = glm::vec4(0.5f,1.0f,1.0f,1.0f), bool visible = false);
	~Tile();
	//Tile render instance helps to give a index in the vector to pass just to render the visibles ones
	//in the spriteRenderer
	void m_setVisible(bool visible,int tileRendereInstance = -1.0f);
	bool m_isVisible();
	void m_setRenderIndex(int index);
};