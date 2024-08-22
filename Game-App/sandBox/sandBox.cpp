#include<glad/gl.h>
#include"sandBox.h"
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<GLFW/glfw3.h>
#include"UI/Ui.h"
#include"Texture/texture.h"
#include<vector>
#include<stb_image/stb_image.h>
#include"entity/entity.h"
#include"Spriterenderer/spriteRenderer.h"
#include"Spriterenderer/rendererDebugQuad/rendererDebugQuad.h"
#include"Spriterenderer/renderTileMap/rendererGridTileMap.h"
#include"debugLog/debugLog.h"
#include"TileMap/TileMap.h"
#include"mathFuncs/mathFuncs.h"
#include"entity/colFunctions/colFunctions.h"


static Camera camera;
static UI gui;
static Shader shaderP;
static Shader shaderIntancing;
static Shader shaderDebugQuad;
static Shader shaderGrid;
static SpriteRenderer* spriteRenderer;
static SpriteRendererInstanced* spriteRendererInstanced;
static rendererDebugQuad* spriteRendererDebugQuad;
static RendererGridTileMap* rendererGridMap;
static SpriteRendererInstanced* spriteRendererInstancedTile;
static TileMap* tileMap;

static std::vector<Texture2D> textures;
static glm::vec3 vel1(0.0f);
static float velocity = 150.0f;
static float animTime = 0.0f;
static std::vector<Vertex> vertices;
static std::vector<Entity2D> entities;
static std::vector<Entity2D_Instaciaded> entitiesInstanced;
static Entity2D_Instaciaded* player;
static glm::vec2 worldGrid(0.0f, 0.0f);
static std::vector<unsigned int> indices;
static glm::vec3 direction(1.0f, 0.0f, 0.0f);
static glm::vec3 globalOrigin(0.0f);


static int instance_index = 0;
static int instanceTileRender_index = 0;
static int instanceEntity_index = 0;
static float scale = 1.0f;
static float widthProjection = 0.0f;
static float heightProjection = 0.0f;
constexpr static float tileSize = 8.0f;


static glm::vec2 getPlayerGridPos(glm::vec2 worlPos)
{
	return glm::vec2(worlPos.x / tileSize, worlPos.y / tileSize);
}

static void Move(Entity2D_Instaciaded& entity,float& dt)
{
	static glm::vec2 remainder(0.0f);
	static bool grounded = false;

	static Rect entityRect = entity.m_getEntityRect();

	// Move on X
	remainder.x += entity.m_speed.x;
	int moveX = round(remainder.x);
	
	if (moveX != 0)
	{
		remainder.x -= moveX;
		int moveSign = byPowerMath::sign(moveX);
		bool collisionHappend = false;

		auto movePlayerX = [&]
			{
				while (moveX)
				{
					entityRect.pos.x += moveSign;
					glm::vec2 entityGridPos = getPlayerGridPos(entity.m_position);
					for (int x = entityGridPos.x - 1; x <= entityGridPos.x + 1; x++)
					{
						for (int y = entityGridPos.y - 1; y <= entityGridPos.y + 1; y++)
						{
							Tile* tile = tileMap->m_GetTile(glm::vec3(x, y, 0.0f));



							if (!tile || !tile->m_isVisible())
							{
								continue;
							}

							Rect tileRect = tile->m_getEntityRect();
							


							if (rect_collision(entityRect, tileRect))
							{
								LOG("COLLISION");
								entity.m_speed.x = 0;
								return;
							}
						}
					}
					entity.m_previusPos = entity.m_position;
					
					entity.m_position.x += moveSign;
					//entity.setPosInterpolation(dt);
					LOGVAR("MOVEX: ", moveX);
					moveX -= moveSign;
				}

			};
		movePlayerX();

	}

	
	entityRect = entity.m_getEntityRect();

	//Move on Y
	remainder.y += entity.m_speed.y;
	int moveY = round(remainder.y);

	if (moveY != 0)
	{
		remainder.y -= moveY;
		int moveSign = byPowerMath::sign(moveY);
		bool collisionHappend = false;

		auto movePlayerY = [&]
			{
				while (moveY)
				{
					entityRect.pos.y += moveSign;
					glm::vec2 entityGridPos = getPlayerGridPos(entity.m_position);
					for (int x = entityGridPos.x - 1; x <= entityGridPos.x + 1; x++)
					{
						for (int y = entityGridPos.y - 1; y <= entityGridPos.y + 1; y++)
						{
							Tile* tile = tileMap->m_GetTile(glm::vec3(x, y, 0.0f));

							if (!tile || !tile->m_isVisible())
							{
								continue;
							}

							Rect tileRect = tile->m_getEntityRect();
							if (rect_collision(entityRect, tileRect))
							{
								if (entity.m_speed.y > 0.0f)
								{
									grounded = true;
								}

								entity.m_speed.y = 0;
								return;
							}
						}
					}
					entity.m_position.y += moveSign;
					moveY -= moveSign;
				}

			};
		movePlayerY();

	}
}

void detectCol(Entity2D_Instaciaded& entity)
{
	glm::vec2 gridPos = tileMap->m_getGridPos(glm::vec2(entity.m_position.x,entity.m_position.y));

	for (int y = gridPos.y - 1; y < gridPos.y + 3; y++)
	{
		for (int x = gridPos.x - 1; x < gridPos.x + 3; x++)
		{
			Tile* tile = tileMap->m_GetGridTile(glm::vec2(x, y));

			
			if (tile && tile->m_isVisible())
			{

				if (rect_collision(entity.m_getEntityRect(), tile->m_getEntityRect()))
				{

					if (entity.m_previusPos.x + entity.m_size.x <= tile->m_position.x &&
						entity.m_position.x + entity.m_size.x > tile->m_position.x)
					{
						entity.m_color = glm::vec4(0.5, 1.0, 0.7, 1.0f);
						entity.m_previusPos.x = tile->m_position.x - entity.m_size.x - 1;
						entity.m_position.x = tile->m_position.x - entity.m_size.x;
						entity.m_wallTouch = true;
						entity.m_right = true;
						entity.m_left = false;
						
					}
					else if (entity.m_previusPos.x >= tile->m_position.x + tile->m_size.x &&
						entity.m_position.x < tile->m_position.x + tile->m_size.x)
					{
						entity.m_color = glm::vec4(0.5, 1.0, 0.7, 1.0f);
						entity.m_previusPos.x = tile->m_position.x + tile->m_size.x + 1;
						entity.m_position.x = tile->m_position.x + tile->m_size.x;
						entity.m_wallTouch = true;
						entity.m_left = true;
						entity.m_right = false;
						
					}
					else if (entity.m_previusPos.y + entity.m_size.y <= tile->m_position.y &&
						entity.m_position.y + entity.m_size.y > tile->m_position.y)
					{
						entity.m_previusPos.y = tile->m_position.y - entity.m_size.y - 1;
						entity.m_position.y = tile->m_position.y - entity.m_size.y;
						entity.m_grounded = true;

					}
					else if (entity.m_previusPos.y >= tile->m_position.y + tile->m_size.y &&
						entity.m_position.y < tile->m_position.y + tile->m_size.y)
					{
						entity.m_previusPos.y = tile->m_position.y + tile->m_size.y + 1;
						entity.m_position.y = tile->m_position.y + tile->m_size.y;

					}


				}
				
				if (entity.m_wallTouch && !entity.m_pushed)
				{
					entity.m_color = glm::vec4(1.0f);
					entity.m_wallTouch = false;
					entity.m_left = false;
					entity.m_right = false;
				}
				if (x == gridPos.x - 1 && y == gridPos.y && entity.m_position.x - 0.1 > tile->m_position.x + tile->m_size.x &&
					entity.m_wallTouch && entity.m_pushed)
				{
					entity.m_wallTouch = false;
					entity.m_left = false;
					entity.m_color = glm::vec4(1.0f);
				}
				else if (x == gridPos.x + 2 && y == gridPos.y && entity.m_position.x + entity.m_size.x + 0.1 < tile->m_position.x &&
					entity.m_wallTouch && entity.m_pushed)
				{
					entity.m_wallTouch = false;
					entity.m_right = false;
					entity.m_color = glm::vec4(1.0f);
				}
				


			}
			
			if (((x == gridPos.x - 1 && y == gridPos.y) && (!tile || !tile->m_isVisible())) && !entity.m_right)
			{
				entity.m_wallTouch = false;
				entity.m_left = false;
				entity.m_color = glm::vec4(1.0f);

			}
			else if (((x == gridPos.x + 2 && y == gridPos.y) && (!tile || !tile->m_isVisible())) && !entity.m_left)
			{
				entity.m_wallTouch = false;
				entity.m_right = false;
				entity.m_color = glm::vec4(1.0f);
			}
			if (y == gridPos.y + 1 && x == gridPos.x && !tile)
				entity.m_grounded = false;
			
			else if (y == gridPos.y + 2 && x == gridPos.x && tile && !tile->m_isVisible() &&
				entity.m_position.y + entity.m_size.y + 0.1 > tile->m_position.y &&
				entity.m_position.x >= tile->m_position.x)
			{
				entity.m_grounded = false;
			}
			
		}
	}
}

SandBox::SandBox()
	:w_width(0), w_height(0), startRender(false),initialZoom(1.0f), m_enableTileMapEditing(false),
	m_showGridMap(false), time(0.0f), m_enableInterpolation(false),m_gravityInfluence(false)
{
	
}

SandBox::~SandBox() {
	//delete spriteRenderer;
	delete spriteRendererDebugQuad;
	delete spriteRendererInstanced;
	delete spriteRendererInstancedTile;
	delete rendererGridMap;
	delete tileMap;
	delete player;
}

void SandBox::initSandBox(int width, int height) {
	this->w_width = width;
	this->w_height = height;
	
	// this projection matrix transform the ndc of opengl to the coords system of the size of the window
	
	widthProjection = (static_cast<float>(this->w_width) / 6);
	heightProjection = (static_cast<float>(this->w_height) / 6);

	glm::mat4 projection = glm::ortho(0.0f, widthProjection,
		heightProjection, 0.0f, -1.0f, 1.0f);
	

	camera.pos = glm::vec3(1.5f, 1.5f, 0.0f);

	worldGrid = glm::vec2(widthProjection / tileSize, heightProjection / tileSize);
	

	
	tileMap = new TileMap(worldGrid, tileSize);

	
	//glm::mat4 persperctiveMat = glm::perspective()
	//this is the main shader for rendering all the entities objecs, player, npcs or enemies, and enviroment objects
	shaderP =  Shader("../shaders/renderer/shader.vs", "../shaders/renderer/shader.frag");
	shaderP.use().setMat4("projection", projection);
	shaderP.use().setInt("textures[0]", 0);
	shaderP.use().setInt("textures[1]", 1);
	
	shaderIntancing = Shader("../shaders/rendererInstancing/Instancing.vs",
		"../shaders/rendererInstancing/renderer.frag");
	shaderIntancing.use().setMat4("projection", projection);
	shaderIntancing.use().setInt("texture0", 0);
	shaderIntancing.use().setInt("texture1", 1);

	shaderDebugQuad = Shader("../shaders/renderDebugQuad/debugQuad.vertex",
		"../shaders/renderDebugQuad/debugQuad.fragment");
	shaderDebugQuad.use().setMat4("projection", projection);
	
	shaderGrid = Shader("../shaders/grid/gridShader.vs",
		"../shaders/grid/gridShader.frag");
	shaderGrid.use().setMat4("projection", projection);



	//spriteRenderer = new SpriteRenderer(shaderP);
	spriteRendererInstanced = new SpriteRendererInstanced(shaderIntancing);
	spriteRendererInstancedTile = new SpriteRendererInstanced(shaderIntancing);
	spriteRendererDebugQuad = new rendererDebugQuad(shaderDebugQuad);
	rendererGridMap = new RendererGridTileMap(shaderGrid,glm::vec4(1.0f));

	this->startRender = false;
	
	

	//entities.reserve(100);
	entitiesInstanced.reserve(100);
	
	vertices.reserve(entities.capacity() * 4);
	indices.reserve(entities.capacity() * 6);

	Texture2D textCat;
	textCat.load("../resources/textures/cat/Cat-Sprite-Sheet.png", true,0);
	Texture2D text2;
	text2.load("../resources/textures/spaceCraft/spaceCraft.png", true,1);

	textures.reserve(2);
	textures.emplace_back(textCat);
	textures.emplace_back(text2);
	

	
	this->initRenderData();
}



void SandBox::initRenderData() {

	

	/*Entity2D entity1((float)textures[0].slot,glm::vec4(1.0f,0.5f,0.4f,1.0f),glm::vec3(40.0f,10.0f,0.0f));
	Entity2D entity2((float)textures[1].slot);
	Entity2D entity3(-1.0f, glm::vec4(0.5f, 1.0f, 0.4f, 1.0f), glm::vec3(10.0f, 10.0f, 0.0f), glm::vec2(5.0f));*/

	player = new Entity2D_Instaciaded(instanceEntity_index, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(16.0f), glm::vec4(1.0f),
		(float)textures[0].slot);
	Entity2D_Instaciaded entity_in2(instanceEntity_index, glm::vec3(150.0f, 50.0f, 0.0f), glm::vec2(8.0f),
		glm::vec4(0.1f,0.5f,1.0f,1.0f), (float)textures[1].slot,glm::vec2(0.0f), glm::vec2(1.0f));
	Entity2D_Instaciaded entity_in3(instanceEntity_index,glm::vec3(100.0f,100.0f,0.0f),glm::vec2(8.0f),
		glm::vec4(0.5f,0.6f,0.7f,1.0f));

	entitiesInstanced.emplace_back(*player);
	entitiesInstanced.emplace_back(entity_in2);
	entitiesInstanced.emplace_back(entity_in3);

	/*for (int i = entitiesInstanced.size() - 1; i < 17; i++) {
		entitiesInstanced.emplace_back(Entity2D_Instaciaded(
			instance_index, (float)textures[0].slot, glm::vec4(0.1f * (i / 2), 0.5f, 1.0f, 1.0f),
			glm::vec3(1.0f + (50.0f * i), 1.0f , 1.0f), glm::vec2(50.0f)
		));
	}*/

	/*entities.emplace_back(entity1); 
	entities.emplace_back(entity2);
	entities.emplace_back(entity3);*/

	

	/*for (int i = 0; i < entities.size(); i++) {

		if (i == 0) {

			entities[i].indices.emplace_back(0);
			entities[i].indices.emplace_back(1);
			entities[i].indices.emplace_back(2);
			entities[i].indices.emplace_back(0);
			entities[i].indices.emplace_back(3);
			entities[i].indices.emplace_back(2);
			
			
		}
		else {
			int j = i * 4;
			entities[i].indices.emplace_back(j);
			entities[i].indices.emplace_back(j + 1);
			entities[i].indices.emplace_back(j + 2);
			entities[i].indices.emplace_back(j);
			entities[i].indices.emplace_back(j + 3);
			entities[i].indices.emplace_back(j + 2);
		}

		//inserting all the vertices to the vertices vector for the draw call
		for (int j = 0; j < 4; j++) {
			vertices.emplace_back(entities[i].vertexs[j]);
		}
		
		for (int j = 0; j < 6; j++) {
			indices.emplace_back(entities[i].indices[j]);
		}
		
	}*/


	//spriteRenderer->reserveRenderBuffer();
	//spriteRenderer->fillData(vertices, indices);
	spriteRendererInstanced->reserveBuffer();
	spriteRendererInstanced->initFillData(entitiesInstanced);
	spriteRendererInstancedTile->reserveBuffer();
	spriteRendererDebugQuad->reserveData();
	spriteRendererDebugQuad->initData(entitiesInstanced);
	rendererGridMap->initBufferData();
	rendererGridMap->fillBufferData(tileSize, tileMap->getWidth(), tileMap->getHeight());
}



void SandBox::processInput(float dt) {
	glm::mat4 model(1.0f);

	if (this->keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyBoard(RIGHT, dt);
	}
	if (this->keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyBoard(LEFT, dt);
	}
	if (this->keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyBoard(UP, dt);
	}
	if (this->keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyBoard(DOWN, dt);
	}
	static float runAcceleration = 240.0f;
	constexpr float runReduce = 280.0f;
	constexpr float gravity = 9.8f;
	if (this->keys[GLFW_KEY_D]) {

		float mult = 1.0f;
		if (player->m_speed.x < 0.0f && player->m_grounded || player->m_speed.x < 0.0f && !this->m_gravityInfluence)
			mult = 100.0f;
		else if (player->m_wallTouch)
			mult = 7.0f;
		else
			mult = 2.0f;

		float actualRunAcceleration = !player->m_grounded && !player->m_wallTouch && this->m_gravityInfluence ?
			runAcceleration / 1.5f : runAcceleration;

		float actualSpeed = !player->m_grounded && !player->m_wallTouch && this->m_gravityInfluence ?
			player->m_maxSpeed / 2 : player->m_maxSpeed / 2 + player->m_maxSpeed / 4;

		actualSpeed = !player->m_grounded && player->m_wallTouch ? player->m_maxSpeed / 2 : actualSpeed;

		player->m_pushed = true;
		player->m_speed.x = byPowerMath::approach(player->m_speed.x,
			actualSpeed, actualRunAcceleration * mult * dt);
		
	}
	if (this->keys[GLFW_KEY_A]) {
		
		float mult = 1.0f;
		if (player->m_speed.x > 0.0f && player->m_grounded || player->m_speed.x > 0.0f && !this->m_gravityInfluence)
			mult = 100.0f;
		else if (player->m_wallTouch)
			mult = 7.0f;
		else
			mult = 2.0f;

		float actualRunAcceleration = !player->m_grounded && !player->m_wallTouch && this->m_gravityInfluence ? 
			runAcceleration / 1.5f : runAcceleration;

		float actualSpeed = !player->m_grounded && !player->m_wallTouch && this->m_gravityInfluence ?
			player->m_maxSpeed / 2: player->m_maxSpeed / 2 + player->m_maxSpeed / 4;

		actualSpeed = !player->m_grounded && player->m_wallTouch ? player->m_maxSpeed / 2 : actualSpeed;

		player->m_pushed = true;
		player->m_speed.x = byPowerMath::approach(player->m_speed.x, 
			-1 * actualSpeed, actualRunAcceleration * mult * dt);
		
	
	}
	if(!this->keys[GLFW_KEY_D] && ! this->keys[GLFW_KEY_A])
	{
		player->m_pushed = false;
		player->m_speed.x = byPowerMath::approach(player->m_speed.x, 0, runReduce * dt);
	
	}
	if (this->keys[GLFW_KEY_W] && !this->m_gravityInfluence) {

		float mult = 1.0f;
		if (player->m_speed.y > 0.0f)

			mult = 100.0f;

		player->m_speed.y = byPowerMath::approach(player->m_speed.y, -player->m_maxSpeed, runAcceleration * mult * dt);

	
	}
	if (this->keys[GLFW_KEY_S] && !this->m_gravityInfluence) {

		float mult = 1.0f;
		if (player->m_speed.y < 0.0f)

			mult = 100.0f;

		player->m_speed.y = byPowerMath::approach(player->m_speed.y, player->m_maxSpeed /2 + player->m_maxSpeed / 4, runAcceleration * mult * dt);
	
	}
	if (this->keys[GLFW_KEY_SPACE] && !this->keyProcessed[GLFW_KEY_SPACE] && this->m_gravityInfluence&& player->m_grounded)
	{
		this->keyProcessed[GLFW_KEY_SPACE] = true;
		player->m_speed.y = -80;
		player->m_grounded = false;
	}
	else if (this->keys[GLFW_KEY_SPACE] && !this->keyProcessed[GLFW_KEY_SPACE] && this->m_gravityInfluence && player->m_wallTouch)
	{
		this->keyProcessed[GLFW_KEY_SPACE] = true;
		player->m_speed.y = -45;
		player->m_speed.x = 0.0f;
		player->m_speed.x = player->m_right ? -100 : 100;
	}
	if (!this->keys[GLFW_KEY_W]  && !this->keys[GLFW_KEY_S] && !this->m_gravityInfluence)
	{
		player->m_speed.y = byPowerMath::approach(player->m_speed.y, 0, runReduce * dt);

	}
	else if (this->m_gravityInfluence && !player->m_grounded) {

		bool get_positiveSpeed = player->m_speed.y >= 0.0f;

		float actualGravity = player->m_wallTouch && get_positiveSpeed ? gravity * 3 : gravity * 12;
		actualGravity = player->m_pushed ? gravity * 15 : actualGravity;

		player->m_speed.y = byPowerMath::approach(player->m_speed.y, player->m_maxSpeed * 2.0f,
			actualGravity * dt);
	}
	else if (this->m_gravityInfluence && player->m_grounded)
		player->m_speed.y = 0.0f;

	
	player->move(dt);
	detectCol(*player);
	spriteRendererInstanced->updateEntity(*player);
	spriteRendererDebugQuad->updateData(*player);
	

	if (this->keys[GLFW_MOUSE_BUTTON_1])
	{	
		//there are two implementations that works the same
		//both implementations make visible the tiles we want with the left click
		//making a tile map
		//the first one is the most complex, just updating the render buffer when we add or remove a tile
		//the second update every iteration the render buffer, that happen in the update method
		//here we just make the tile visible

		if (this->m_enableTileMapEditing)
		{

			tileMap->m_addTileToRender(glm::vec3(this->m_mousePosGlobal, 0.0f));
			tileMap->m_updateTileRenderBuffer(*spriteRendererInstancedTile);
		
			//tileMap->m_addTile(glm::vec3(this->m_mousePosGlobal, 0.0f));
		}
	}
	if (this->keys[GLFW_MOUSE_BUTTON_RIGHT] )
	{
		if (this->m_enableTileMapEditing)
		{
			tileMap->m_deleteTileFromRender(glm::vec3(this->m_mousePosGlobal, 0.0f));
			tileMap->m_updateTileRenderBuffer(*spriteRendererInstancedTile);


			//tileMap->m_removeTile(glm::vec3(this->m_mousePosGlobal, 0.0f));
		}
	}
}

void SandBox::setCameraScroll(float yoffset) {

	camera.processMouseScroll(yoffset);

}

void SandBox::GetMousePos(glm::vec2 pos)
{	
	this->m_mousePosScreen = glm::vec2(pos.x / 6, pos.y / 6);
	
}

void SandBox::updateMousePos()
{
	//get the global pos of the mouse relative with the camera zoom
	glm::vec3 diff = globalOrigin - camera.pos;
	glm::vec3 mouseGlobalPos = glm::vec3(this->m_mousePosScreen, 0.0f) - diff;
	this->m_mousePosGlobal = glm::vec2(mouseGlobalPos.x, mouseGlobalPos.y) / camera.Zoom;
}

void SandBox::update(float& dt) {
	this->updateMousePos();
	this->updateEntities(dt);
	this->time += dt;

	
	// the next commented line update the render buffer to render tiles
	// it goes throught a matrix to see the tiles that are visible and add to a list
	// to pass to feed the render buffer
	//tileMap->m_updateTilesBufferRenderer(*spriteRendererInstancedTile);

	if (player->m_texCoords.position.x >= 96.0f)
		player->m_texCoords.position.x = 0.0f;
	else if (animTime > 0.5f) {
		animTime = 0.0f;
		player->setNewTexOffset(glm::vec2(32.0f, 0.0f));
		spriteRendererInstanced->updateEntity(*player);
	}
	else {
		animTime += 6.0f * dt;
	}
	
	
	//spriteRendererInstanced->scaleSingleEntity(entitiesInstanced[0], scale,camera.Zoom);
}

//This update all the entities where the player has no control,npcs or enemies
void SandBox::updateEntities(float& dt) {

	float vel = dt * velocity / 1.5;
	
	/*this->updateVertices(entities[0], 0);
	if (entities[1].position.x + entities[1].size.x > this->w_width/this->initialZoom || entities[1].position.x < 0)
		direction.x *= -1;
	entities[1].position.x += vel * direction.x;
	entities[1].updateVertexs();
	this->updateVertices(entities[1], 1);*/

}

//this func update the vertices in the vertices vector for a specific entity 
void SandBox::updateVertices(Entity2D& entity, int entityIndex) {

	int VerticeIndex = entityIndex * 4;	
	
	for (int i = 0; i < 4; i++) {
		vertices[VerticeIndex + i].position = entity.vertexs[i].position;
		vertices[VerticeIndex + i].texCoords = entity.vertexs[i].texCoords;
	}
	std::vector<Vertex> verticesToUpdate;
	verticesToUpdate.reserve(4);
	verticesToUpdate.emplace_back(entity.vertexs[0]);
	verticesToUpdate.emplace_back(entity.vertexs[1]);
	verticesToUpdate.emplace_back(entity.vertexs[2]);
	verticesToUpdate.emplace_back(entity.vertexs[3]);
	spriteRenderer->updateData(verticesToUpdate, entityIndex);
	
}

void SandBox::renderGUI() {

	
	
	//gui.useSliderForVec2("Vec for change y and x texCoords", entities[0].maxTexCoords);
	gui.showVec("player pos", player->m_position);
	gui.showVec("player speed", player->m_speed);
	gui.showVec("camera", camera.pos);
	
	gui.showVec("globalMousePos", glm::vec3(this->m_mousePosGlobal,0.0f));
	gui.enableTileMapEditing(this->m_enableTileMapEditing, this->m_showGridMap);
	gui.enableBool("gravity influence", this->m_gravityInfluence);
	gui.showBoolVar("Pushed player", player->m_pushed);
	gui.showBoolVar("Wall Touch player", player->m_wallTouch);
	float xRightPos = player->m_position.x + player->m_size.x;
	
	//gui.enableBool("showGrid", this->m_showGridMap);
	//gui.useSliderForFloat("scale", scale);
	
	

}

void SandBox::renderScene(float dt) {

	//spriteRenderer->draw(vertices, indices, camera, textures);
	
	if (m_showGridMap)
		rendererGridMap->draw(camera);
	spriteRendererInstancedTile->draw(camera, textures, "renderTiles");
	spriteRendererInstanced->draw(camera,textures,"renderEntities");
	spriteRendererDebugQuad->draw(camera);
}

