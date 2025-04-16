#pragma once
#include<glm/glm.hpp>
#include<vector>
#include<string>
#include"Texture/texture.h"

/*	Simple vertex Structure to pass render data of a vertex
	This is not use to much in my interface of instance rendering but I leave it because maybe it 
	will help me in the future and to see the first implemetation of batch rendering in the SpriteRenderer
	class that for now it's not updated to handle the polymorfism for classes that would inheret from 
	Entity2D
*/
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
	bool col;

	Rect(glm::vec2 _pos = glm::vec2(0.0f), glm::vec2 _size = glm::vec2(0.0f),
		glm::vec2 _prevPos = glm::vec2(0.0f), glm::vec2 _posOffset = glm::vec2(0.0f),
		glm::vec2 _sizeOffset = glm::vec2(0.0f))
		:pos(_pos), size(_size), previusPos(_prevPos), posOffset(_posOffset), sizeOffset(_sizeOffset), col(false)
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
	WALL_TOUCH,
};
/*
	this class is for use of batch rendering but I desisted of doing in that way just to understand how
	to use instance rendering in OpenGL but i will probably write a system of rendering and improve how
	to handle the data for a Entity2D just to stablish parameters and make it running without complications
	
*/
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


/*
	Entity2D_Instaciaded is the base class for entities used in the sandBox class,
	the Player and EnemySlime class are in the same project for simplicity, but probably the better way
	to structure the project would be to put your own code in the main app directories to compile.
	
	animTimeLimit is how much time have to pass to change the frame in the texture of the entity
	if the time pass we will change the coords, i will make a better interface to make it more easy
	but for now i leave it this way.

	there are two classes that inheret from this one and use that kind of system but coded manually:
	Player and EnemySlime (in the Enemy.h and Enemy.cpp respectly for the EnemySlime).

	I would like to set a better interface to make it more flexible and easy to use and costumize but
	for now I leave it like this.
*/

class Entity2D_Instaciaded {

public:
	float texSlot;
	float m_maxSpeed;
	float m_animTimeLimit;
	float m_animTime;
	glm::vec3 m_speed;
	glm::vec3 m_position;
	glm::vec3 m_previusPos;
	glm::vec3 m_direction;
	glm::vec2 m_size;
	glm::vec4 m_color;
	TexCoords m_texCoords;
	glm::vec2 m_texOffset;
	glm::vec2 m_posOffsetRect;
	glm::vec2 m_sizeOffsetRect;
	int m_entityIndex;
	int m_renderInstanceIndex;
	EntityState state;
	std::string m_tagName;
	
	/*
		grounded is use for detection of the "ground" and m_bottom should be use for collision detection
		on the bottom, but if you want, can use as you wish
	*/
	bool m_updateTimers;
	bool m_grounded;
	bool m_wallTouch;
	bool m_pushed;
	bool m_right;
	bool m_left;
	bool m_top;
	bool m_bottom;
	bool m_col;
	bool m_moveRoutine;
	bool m_gravityInfluence;
	bool m_delete;
	bool m_toDelete;

	Entity2D_Instaciaded(int* renderIndex = nullptr, glm::vec3 position = glm::vec3(0.0f), glm::vec2 size = glm::vec2(10.0f),
		glm::vec4 color = glm::vec4(1.0f), glm::vec2 posOffsetRect = glm::vec2(0.0f), glm::vec2 sizeOffsetRect = glm::vec2(0.0f), float texSlot = -1.0f, glm::vec2 texPos = glm::vec2(0.0f),
		glm::vec2 texSize = glm::vec2(16.0f));

	~Entity2D_Instaciaded();

	void setNewTexOffset(glm::vec2 newOffset);
	int m_returnRenderIndex();
	void setPosInterpolation(float& dt);
	
	virtual void m_onCollision();
	virtual void move(float& dt);
	virtual void m_anim(float& dt);
	virtual Rect m_getEntityRect();
	virtual std::string m_getTagName();
	virtual void m_updateTimer(float& dt);
	
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