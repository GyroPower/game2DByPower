#include"entity.h"
#include"debugLog/debugLog.h"
#include"mathFuncs/mathFuncs.h"
#include"TileMap/TileMap.h"

Entity2D::Entity2D(float texSlot, glm::vec4 color, glm::vec3 position, glm::vec2 size)
	: color(color), position(position), size(size), texSlot(texSlot)
{
	this->vertexs.reserve(4);
	this->indices.reserve(6);
	this->maxTexCoords = glm::vec2(1.0f);


	//bottom left
	this->vertexs.emplace_back(
		Vertex(glm::vec3(this->position.x,this->position.y + this->size.y,0.0f), glm::vec2(0.0f),
			this->color,texSlot));

	//bottom right
	this->vertexs.emplace_back(
		Vertex(glm::vec3(this->position.x + this->size.x, this->position.y + this->size.y, 0.0f),
			glm::vec2(1.0f, 0.0f), this->color, this->texSlot));
	
	//top right
	this->vertexs.emplace_back(
		Vertex(glm::vec3(this->position.x + this->size.x, this->position.y, 0.0f),
			glm::vec2(1.0f, 1.0f), this->color, this->texSlot));
	
	//top left
	this->vertexs.emplace_back(
		Vertex(this->position, glm::vec2(0.0f, 1.0f), this->color, this->texSlot));

}

void Entity2D::updateVertexs() {
	this->vertexs[0].position = glm::vec3(this->position.x, this->position.y + this->size.y, 0.0f);
	this->vertexs[1].position = glm::vec3(this->position.x + this->size.x,
		this->position.y + this->size.y, 0.0f);
	this->vertexs[2].position = glm::vec3(this->position.x + this->size.x, this->position.y, 0.0f);
	this->vertexs[3].position = glm::vec3(this->position.x, this->position.y, 0.0f);
}

void Entity2D::updateTopRightTexCoords() {
	this->vertexs[1].texCoords = glm::vec2(this->maxTexCoords.x, 0.0f);
	this->vertexs[2].texCoords = this->maxTexCoords;
	this->vertexs[3].texCoords = glm::vec2(0.0f, this->maxTexCoords.y);
}


/////////////////////////////
//Entity2D instanced class
////////////////////////////

//This is to have seperate the render and entity index, if a entity exists but is not redered well, the render index
//is set to other value, just to have track of the gpu memory and change it if the entity is visible or not and render
//it
static int entityIndex = 0;

Entity2D_Instaciaded::Entity2D_Instaciaded(int* renderIndex, glm::vec3 position, glm::vec2 size,
	glm::vec4 color, glm::vec2 posOffsetRect, glm::vec2 sizeOffsetRect, float texSlot, glm::vec2 texPos,
	glm::vec2 texSize)
	: m_color(color), m_position(position), m_size(size), texSlot(texSlot), m_entityIndex(entityIndex),
	m_texCoords(texPos, texSize), m_texOffset(glm::vec2(0.0f)), m_maxSpeed(120.0f), m_previusPos(position),
	m_grounded(false), m_speed(0.0f), m_wallTouch(false), m_pushed(false), m_right(false), m_left(false), state(IDLE),
	m_posOffsetRect(posOffsetRect), m_sizeOffsetRect(sizeOffsetRect), m_animTimeLimit(0.5f), m_animTime(0.0f), m_top(false),
	m_col(false), m_tagName(""), m_bottom(false), m_updateTimers(false), m_moveRoutine(false), m_delete(false),
	m_toDelete(false)
{
	this->m_direction = glm::vec3(0.0f);

	if (renderIndex)
	{

		this->m_renderInstanceIndex = *renderIndex;
		
		*renderIndex+=1;
	}
	else
		this->m_renderInstanceIndex = 0;

	entityIndex++;
	
	this->m_texCoords.position += m_texOffset;
}

Entity2D_Instaciaded::~Entity2D_Instaciaded()
{
}

void Entity2D_Instaciaded::setNewTexOffset(glm::vec2 newOffset) {
	this->m_texOffset = newOffset;
	this->m_texCoords.position += this->m_texOffset;
}

int Entity2D_Instaciaded::m_returnRenderIndex()
{
	return this->m_renderInstanceIndex;
}

void Entity2D_Instaciaded::move(float& dt)
{
	/* Basic way of how should you entity move, can change if you want in a children class */
	this->m_previusPos = this->m_position;
	this->m_position += this->m_speed;
	this->setPosInterpolation(dt);
}

void Entity2D_Instaciaded::setPosInterpolation(float& dt)
{
	this->m_position = byPowerMath::lerp(this->m_previusPos, this->m_position, dt);
}

Rect Entity2D_Instaciaded::m_getEntityRect()
{
	/* 
		Basic way of how should you entity return the rect for collision detection,
		can change if you want in a children class 
	*/
	return Rect(this->m_position,this->m_size,this->m_previusPos);
}

std::string Entity2D_Instaciaded::m_getTagName()
{
	return m_tagName;
}

void Entity2D_Instaciaded::m_onCollision()
{
	/*
		set the logic of how should react on collision on your children class
	*/
}


void Entity2D_Instaciaded::m_anim(float& dt)
{
	/*
		set the logic of how should animate and update the texCoords on your children class
	*/
}

void Entity2D_Instaciaded::m_updateTimer(float& dt)
{
	/*
		set the logic for timers in the child classes
	*/
}

////////////////////////////////////
//// Tile class
///////////////////////////////////

Tile::Tile(int* renderIndex,glm::vec3 pos, glm::vec2 size, glm::vec4 color,bool visible) 
	:m_visible(visible), Entity2D_Instaciaded(renderIndex, pos, size, color)
{

}

void Tile::m_setVisible(bool visible,int tileRenderInstace)
{
	this->m_visible = visible;

	if (visible)
	{
		this->m_renderInstanceIndex = tileRenderInstace;
		
	}
	else 
	{
		this->m_renderInstanceIndex = tileRenderInstace;
		
	}
}

bool Tile::m_isVisible() 
{
	return this->m_visible;
}


void Tile::m_setRenderIndex(int index)
{
	this->m_renderInstanceIndex = index;
}


Tile::~Tile() { }