#include<glad/gl.h>
#include"sandBox.h"
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<GLFW/glfw3.h>
#include"GUI/Gui-Debug.h"
#include"Texture/texture.h"
#include<vector>
#include<stb_image/stb_image.h>
#include"entity/entity.h"
#include"entity/player/player.h"
#include"entity/Enemy/Enemy.h"
#include"Spriterenderer/spriteRenderer.h"
#include"Spriterenderer/rendererDebugQuad/rendererDebugQuad.h"
#include"Spriterenderer/renderTileMap/rendererGridTileMap.h"
#include"debugLog/debugLog.h"
#include"TileMap/TileMap.h"
#include"mathFuncs/mathFuncs.h"
#include"entity/colFunctions/colFunctions.h"


static Camera* camera;
static GUI_DEBUG gui;
static Shader shaderEntities;
static Shader shaderDebugQuad;
static Shader shaderGrid;
static Shader shaderPlayer;
static SpriteRendererInstanced* spriteRendererPlayer;
static SpriteRendererInstanced* spriteRendererEntities;
static rendererDebugQuad* spriteRendererDebugQuad;
static rendererDebugQuad* spriteRendererDebugQuadPlayer;
static RendererGridTileMap* rendererGridMap;
static SpriteRendererInstanced* spriteRendererInstancedTile;
static TileMap* tileMap;

static std::vector<Texture2D> textures;
static float velocity = 150.0f;
static float animTime = 0.0f;
static std::vector<Entity2D_Instaciaded*> entitiesInstanced;
static Player* player;
static glm::vec2 worldGrid(0.0f, 0.0f);
static glm::vec3 globalOrigin(0.0f);
static std::vector<Texture2D> playerTex;
static std::vector<Texture2D> tilesTex;
static std::vector<Tile> tilesChangeColor;
static std::vector<Tile> tilesReturnColor;
static std::vector<Shader> shadersPrograms;

static int initialSizeChange = 0;
static int initialSizeReturn = 0;

static int instanceTileRender_index = 0;
static int instanceEntity_index = 0;
static int playerRenderIndex = 0;
static float scale = 1.0f;
static float widthProjection = 0.0f;
static float heightProjection = 0.0f;
static float tileSize = 8.0f;
static float hitTime = 0.0f;
static float hitTimeDur = 3.0f;


static glm::vec2 getPlayerGridPos(glm::vec2 worlPos)
{
	return glm::vec2(worlPos.x / tileSize, worlPos.y / tileSize);
}


void callEntityVirtualFunct(Entity2D_Instaciaded& entity)
{
	entity.m_getEntityRect();
	
}


/*
	This next function is to check simply for simply tile detection for entities that have simply 
	movement, like the enemy slime just to move in the x axis
*/
void checkEntitiesTileLimits(Entity2D_Instaciaded& entity, bool& gravityInfluence, float& dt)
{
	glm::vec2 gridPos = tileMap->m_getGridPos(glm::vec2(entity.m_getEntityRect().pos));
	glm::vec2 gridPosSize = tileMap->m_getGridPos(glm::vec2(entity.m_getEntityRect().pos + entity.m_getEntityRect().size));
	if (gridPos.y < 0.0f)
		gridPos.y = (int)gridPos.y - 1;
	if (gridPos.x < 0.0f)
		gridPos.x = (int)gridPos.x - 1;
	if (gridPosSize.x < 0.0f)
		gridPosSize.x = (int)gridPosSize.x - 1;
	if (gridPosSize.y < 0.0f)
		gridPosSize.y = (int)gridPosSize.y - 1;

	gridPos.y = (int)gridPos.y;
	gridPos.x = (int)gridPos.x;
	gridPosSize.y = (int)gridPosSize.y;
	gridPosSize.x = (int)gridPosSize.x;

	
	// see if there are tiles in the left
	Tile* tileTopLeft = tileMap->m_GetGridTile(gridPos);
	Tile* tileBottomLeft = tileMap->m_GetGridTile(glm::vec2(gridPos.x, gridPosSize.y));

	// see if there are tiles in the right
	Tile* tileTopRight = tileMap->m_GetGridTile(glm::vec2(gridPosSize.x, gridPos.y));
	Tile* tileBottomRight = tileMap->m_GetGridTile(glm::vec2(gridPosSize));
	
	if (tileTopLeft && tileTopLeft->m_isVisible() && entity.m_direction.x == -1)
		entity.m_direction.x = 1;
	else if (tileBottomLeft && tileBottomLeft->m_isVisible() && entity.m_direction.x == -1)
		entity.m_direction.x = 1;
	else if (tileTopRight && tileTopRight->m_isVisible() && entity.m_direction.x == 1)
		entity.m_direction.x = -1;
	else if (tileBottomRight && tileBottomRight->m_isVisible() && entity.m_direction.x == 1)
		entity.m_direction.x = -1;
	if (entity.m_gravityInfluence && gravityInfluence)
	{

		if (!entity.m_grounded)
		{
			entity.m_speed.y = byPowerMath::approach(entity.m_speed.y, entity.m_maxSpeed, dt * 9.8f);
			entity.m_direction.y = 1;
		}

		Tile* tileDownLeft = tileMap->m_GetGridTile(glm::vec2(gridPos.x, gridPosSize.y));
		Tile* tileDownRight = tileMap->m_GetGridTile(glm::vec2(gridPosSize.x, gridPosSize.y));
		
		float offsetYprev = 1.0f;
		float offsetY = 0.5;

		// I give offsets for the y component to make it simpler to detect if there is a tile down the entity and use it to check if there is not 
		// is not grounded and the gravity affects it 
		Rect entityRect = entity.m_getEntityRect();
		Rect tileDownLeftRect = tileDownLeft->m_getEntityRect();
		Rect tileDownRightRect = tileDownRight->m_getEntityRect();
		if (tileDownLeft && tileDownLeft->m_isVisible() && rect_collision(entityRect, tileDownLeftRect))
		{
			if (entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y > tileDownLeft->m_getEntityRect().pos.y &&
				entity.m_getEntityRect().previusPos.y + entity.m_getEntityRect().size.y <= tileDownLeft->m_getEntityRect().pos.y ||
				entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y < tileDownLeft->m_getEntityRect().pos.y + (tileDownLeft->m_getEntityRect().size.y / 2) &&
				entity.m_getEntityRect().previusPos.y + entity.m_getEntityRect().size.y > tileDownLeft->m_getEntityRect().pos.y)
			{

				entity.m_speed.y = 0.0f;
				entity.m_direction.y = 0;
				entity.m_grounded = true;
				entity.m_previusPos.y = tileDownLeft->m_getEntityRect().pos.y - entity.m_getEntityRect().size.y -
					entity.m_getEntityRect().posOffset.y - offsetYprev;
				entity.m_position.y = tileDownLeft->m_getEntityRect().pos.y - entity.m_getEntityRect().size.y -
					entity.m_getEntityRect().posOffset.y - offsetY;
			}
		}
		else if (tileDownRight && tileDownRight->m_isVisible() && rect_collision(entityRect, tileDownRightRect))
		{
			if (entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y > tileDownRight->m_getEntityRect().pos.y &&
				entity.m_getEntityRect().previusPos.y + entity.m_getEntityRect().size.y <= tileDownRight->m_getEntityRect().pos.y ||
				entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y < tileDownRight->m_getEntityRect().pos.y + (tileDownRight->m_getEntityRect().size.y / 2) &&
				entity.m_getEntityRect().previusPos.y + entity.m_getEntityRect().size.y > tileDownRight->m_getEntityRect().pos.y)
			{
				entity.m_speed.y = 0.0f;
				entity.m_direction.y = 0;
				entity.m_grounded = true;
				entity.m_previusPos.y = tileDownRight->m_getEntityRect().pos.y - entity.m_getEntityRect().size.y -
					entity.m_getEntityRect().posOffset.y - offsetYprev;
				entity.m_position.y = tileDownRight->m_getEntityRect().pos.y - entity.m_getEntityRect().size.y -
					entity.m_getEntityRect().posOffset.y - offsetY;

			}
		}
		else
		{
			gridPos = tileMap->m_getGridPos(glm::vec2(entityRect.pos + offsetY));
			gridPosSize = tileMap->m_getGridPos(glm::vec2((entityRect.pos + offsetY) + entityRect.size));
			if (gridPos.y < 0.0f)
				gridPos.y = (int)gridPos.y - 1;
			if (gridPos.x < 0.0f)
				gridPos.x = (int)gridPos.x - 1;
			if (gridPosSize.x < 0.0f)
				gridPosSize.x = (int)gridPosSize.x - 1;
			if (gridPosSize.y < 0.0f)
				gridPosSize.y = (int)gridPosSize.y - 1;

			gridPos.y = (int)gridPos.y;
			gridPos.x = (int)gridPos.x;
			gridPosSize.y = (int)gridPosSize.y;
			gridPosSize.x = (int)gridPosSize.x;

			tileDownLeft = tileMap->m_GetGridTile(glm::vec2(gridPos.x, gridPosSize.y));
			tileDownRight = tileMap->m_GetGridTile(glm::vec2(gridPosSize.x, gridPosSize.y));
			
			if ((tileDownLeft && !tileDownLeft->m_isVisible() || !tileDownLeft) && (tileDownRight && !tileDownRight->m_isVisible() || !tileDownRight))
				entity.m_grounded = false;
		}
		
	}

}


/*
	this is to detect collision with two different entities, and just check for the player and other entities, and for better performance I should 
	create an interface to handle the most near interactions that could happend, making it less checks for collisions
*/

void detectEntityCol(Entity2D_Instaciaded& entity1, Entity2D_Instaciaded& entity2, float& dt)
{
	
	if (entity2.m_updateTimers || entity1.m_updateTimers)
	{
		entity2.m_updateTimer(dt);
		entity1.m_updateTimer(dt);
	}
	Rect entity1Rect = entity1.m_getEntityRect();
	Rect entity2Rect = entity2.m_getEntityRect();
	
	if (rect_collision(entity1Rect, entity2Rect) && 
		!entity1.m_toDelete && !entity2.m_toDelete)
	{


		if (entity1.m_getTagName() == "player" && entity2.m_getTagName() == "slime")
		{
			
			if ((entity1.m_getEntityRect().previusPos.y + entity1.m_getEntityRect().size.y <= entity2.m_getEntityRect().pos.y &&
				entity1.m_getEntityRect().pos.y + entity1.m_getEntityRect().size.y > entity2.m_getEntityRect().pos.y ||
				entity1.m_getEntityRect().previusPos.y + entity1.m_getEntityRect().size.y < entity2.m_getEntityRect().pos.y + (entity2.m_getEntityRect().size.y / 2) &&
				entity1.m_getEntityRect().pos.y + entity1.m_getEntityRect().size.y > entity2.m_getEntityRect().pos.y) && !entity2.m_updateTimers)
				{
					entity1.m_col = true;
					entity1.m_bottom = true;
					entity2.m_col = true;
					entity2.m_top = true;
					entity2.m_updateTimers = true;
				}
			else if (((entity1.m_getEntityRect().pos.y < entity2.m_getEntityRect().pos.y + entity2.m_getEntityRect().size.y &&
				entity1.m_getEntityRect().previusPos.y >= entity2.m_getEntityRect().pos.y + entity2.m_getEntityRect().size.y) ||
				(entity1.m_getEntityRect().pos.y > entity2.m_getEntityRect().pos.y + (entity2.m_getEntityRect().size.y / 2) &&
					entity1.m_getEntityRect().previusPos.y < entity2.m_getEntityRect().pos.y + entity2.m_getEntityRect().size.y)) &&
				!entity1.m_updateTimers)
			{
				entity1.m_col = true;
				entity1.m_top = true;
				entity1.m_updateTimers = true;
				entity2.m_col = true;
				
			}
			else if (((entity1.m_getEntityRect().pos.x + entity1.m_getEntityRect().size.x > entity2.m_getEntityRect().pos.x &&
				entity1.m_getEntityRect().previusPos.x + entity1.m_getEntityRect().size.x < entity2.m_getEntityRect().pos.x) ||
				(entity1.m_getEntityRect().previusPos.x + entity1.m_getEntityRect().size.x > entity2.m_getEntityRect().pos.x &&
					entity1.m_getEntityRect().pos.x < entity2.m_getEntityRect().pos.x + (entity2.m_getEntityRect().size.x / 2))) &&
				!entity1.m_updateTimers)
				{
					entity1.m_col = true;
					entity1.m_right = true;
					entity1.m_updateTimers = true;
					entity2.m_col = true;
				}
			else if (((entity1.m_getEntityRect().pos.x < entity2.m_getEntityRect().pos.x + entity2.m_getEntityRect().size.x &&
				entity1.m_getEntityRect().previusPos.x >= entity2.m_getEntityRect().pos.x + entity2.m_getEntityRect().size.x) ||
				(entity1.m_getEntityRect().previusPos.x < entity2.m_getEntityRect().pos.x + entity2.m_getEntityRect().size.x &&
					entity1.m_getEntityRect().pos.x > entity2.m_getEntityRect().pos.x + (entity2.m_getEntityRect().size.x / 2))) && !entity1.m_updateTimers)
			{
				entity1.m_col = true;
				entity1.m_left = true;
				entity1.m_updateTimers = true;
				entity2.m_col = true;
			}

				entity1.m_onCollision();
				entity2.m_onCollision();
				

			
		}
	}
	
}

/*
	detectTileCol like the name say's it's for that for entities, and it's not flexible,
	because if I change the size of the tile map, it will break cause, I check tiles under the player
	making offsets for his size and if I change the tile map size it will no longer usefull, so for a better
	flexible tile collision detection I should change it to take into account the size of the tile map
	and how big is the entity and how much tiles it would be take his size to make the right offsets to do
	the correct tile collision detection, but for simplicity I leave this way.

	And i could doit more simpler but it was the first way of how I manage the tile collision with the player,
	I will change it.

	And other thing, for games like 2d zelda games, like A Link to the past, it would be differect collision
	detection, cause some tiles would be the ones that collides with the player and other don't, so, to do
	something like that it would take to consider a different collision system, and make others tile classes
	like "TileGround, TileDecoration, TileObstacle" for the enviroment or do a parent Tile class flexible
	to do it more quickly and easey to developt a game like that
*/


void detectTileCol(Entity2D_Instaciaded& entity)
{
	glm::vec2 gridPos = tileMap->m_getGridPos(glm::vec2(entity.m_getEntityRect().pos));

	if (gridPos.y < 0.0f)
		gridPos.y = (int)gridPos.y - 1;
	if (gridPos.x < 0.0f)
		gridPos.x = (int)gridPos.x - 1;
	
	gridPos.y = (int)gridPos.y;
	gridPos.x = (int)gridPos.x;

	// check for every tile around the player for collision
	for (int y = gridPos.y - 1; y < gridPos.y + 3; y++)
	{
		for (int x = gridPos.x - 1; x < gridPos.x + 3; x++)
		{
			Tile* tile = tileMap->m_GetGridTile(glm::vec2(x, y));


			if (tile && tile->m_isVisible())
			{

				Rect entityRect = entity.m_getEntityRect();
				Rect tileRect = tile->m_getEntityRect();	
				// check collisions
				if (rect_collision(entityRect, tileRect))
				{
					
					// check if came from the bottom

					
					if ((entity.m_getEntityRect().previusPos.y + entity.m_getEntityRect().size.y <= tile->m_getEntityRect().pos.y &&
						entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y > tile->m_getEntityRect().pos.y) ||
						(entity.m_getEntityRect().previusPos.y + entity.m_getEntityRect().size.y < tile->m_getEntityRect().pos.y + (tile->m_getEntityRect().size.y / 4) &&
							entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y > tile->m_getEntityRect().pos.y) && entity.m_speed.y >= 0.0f)
					{

						entity.m_grounded = true;
						entity.m_previusPos.y = tile->m_getEntityRect().pos.y - entity.m_getEntityRect().size.y -
							entity.m_getEntityRect().posOffset.y - 0.5f;
						entity.m_position.y = tile->m_getEntityRect().pos.y - entity.m_getEntityRect().size.y -
							entity.m_getEntityRect().posOffset.y;
					}
					//check if was on top
					else if (
						((entity.m_getEntityRect().previusPos.y >=
							tile->m_getEntityRect().pos.y + tile->m_getEntityRect().size.y &&
							entity.m_getEntityRect().pos.y < tile->m_getEntityRect().pos.y + tile->m_getEntityRect().size.y) &&
							entity.m_speed.y <= 0.0f))
					{
						entity.m_top = true;
						entity.m_previusPos.y = tile->m_position.y + tile->m_size.y - entity.m_getEntityRect().posOffset.y + 0.5f;
						entity.m_position.y = tile->m_position.y + tile->m_size.y - entity.m_getEntityRect().posOffset.y;
					}
					//check if was on left
					else if (entity.m_getEntityRect().previusPos.x >= tile->m_getEntityRect().pos.x + tile->m_getEntityRect().size.x &&
						entity.m_getEntityRect().pos.x < tile->m_getEntityRect().pos.x + tile->m_getEntityRect().size.x ||
						entity.m_getEntityRect().previusPos.x > tile->m_getEntityRect().pos.x + (tile->m_getEntityRect().size.x / 2) &&
						entity.m_getEntityRect().pos.x < tile->m_getEntityRect().pos.x + tile->m_getEntityRect().size.x)
					{
						entity.m_previusPos.x = tile->m_getEntityRect().pos.x + tile->m_getEntityRect().size.x - entity.m_getEntityRect().posOffset.x + 0.5f;
						entity.m_position.x = tile->m_getEntityRect().pos.x + tile->m_getEntityRect().size.x - entity.m_getEntityRect().posOffset.x;
						entity.m_left = true;
						entity.m_right = false;
						entity.m_wallTouch = true;
					}
					//check if was on right
					else if (entity.m_getEntityRect().previusPos.x + entity.m_getEntityRect().size.x <= tile->m_getEntityRect().pos.x &&
						entity.m_getEntityRect().pos.x + entity.m_getEntityRect().size.x > tile->m_getEntityRect().pos.x ||
						entity.m_getEntityRect().previusPos.x + entity.m_getEntityRect().size.x < tile->m_getEntityRect().pos.x + (tile->m_getEntityRect().size.x / 2) &&
						entity.m_getEntityRect().pos.x + entity.m_getEntityRect().size.x > tile->m_getEntityRect().pos.x)
					{
						entity.m_previusPos.x = tile->m_position.x - entity.m_getEntityRect().posOffset.x - entity.m_getEntityRect().size.x - 0.5f;
						entity.m_position.x = tile->m_position.x - entity.m_getEntityRect().posOffset.x - entity.m_getEntityRect().size.x;
						entity.m_right = true;
						entity.m_left = false;
						entity.m_wallTouch = true;
					}

				}

				// if not collision, see if the entity is still grounded
				if (y == gridPos.y + 2 && (x == gridPos.x || x == gridPos.x + 1) && entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y + 0.1 <
					tile->m_getEntityRect().pos.y)
				{
					entity.m_grounded = false;
				}
				else if (y == gridPos.y - 1 && entity.m_getEntityRect().pos.y - 0.1f > tile->m_getEntityRect().pos.y + tile->m_getEntityRect().size.y)
					entity.m_top = false;
				
				if (entity.m_wallTouch && !entity.m_pushed)
				{
					entity.m_wallTouch = false;
					entity.m_left = false;
					entity.m_right = false;
				}

			}
			else if (y == gridPos.y - 1 && x == gridPos.x && (tile && !tile->m_isVisible()))
			{
				Tile* anotherTile = tileMap->m_GetGridTile(glm::vec2(gridPos.x + 1,y));

				if (anotherTile && !anotherTile->m_isVisible() || anotherTile == nullptr)
					entity.m_top = false;
			}
			//see if the entity still have tiles down, if not, is not grounded anymore
			else if (y == gridPos.y + 2 && x == gridPos.x && ((tile && !tile->m_isVisible()) || tile == nullptr))
			{
				Tile* anotherTile = tileMap->m_GetGridTile(glm::vec2(gridPos.x + 1, gridPos.y + 2));

				if ((anotherTile && !anotherTile->m_isVisible()) || anotherTile == nullptr)
				{
					entity.m_grounded = false;
				}
			}
			// if theres is not a wall in both sides of the player
			else if ((y == gridPos.y) && x == gridPos.x - 1 && ((tile && !tile->m_isVisible()) || tile == nullptr))
			{
				Tile* anotherTile = tileMap->m_GetGridTile(glm::vec2(gridPos.x + 2,y));
				if ((anotherTile && !anotherTile->m_isVisible()) || anotherTile == nullptr)
				{
					entity.m_left = false;
					entity.m_right = false;
					entity.m_wallTouch = false;
				}

			}
		}
	}
}

SandBox::SandBox()
	:w_width(0), w_height(0), startRender(false),initialZoom(1.0f), m_enableTileMapEditing(false),
	m_showGridMap(false), time(0.0f), m_showEntitiesQuads(false),m_gravityInfluence(false)
{
	
}

SandBox::~SandBox() {
	//delete spriteRenderer;
	delete spriteRendererDebugQuad;
	delete spriteRendererPlayer;
	delete spriteRendererDebugQuadPlayer;
	delete spriteRendererEntities;
	delete spriteRendererInstancedTile;
	delete rendererGridMap;
	delete tileMap;
	delete player;
	delete camera;
	entitiesInstanced.clear();
}

void SandBox::initSandBox(int width, int height) {
	this->w_width = width;
	this->w_height = height;
	
	#ifdef DEBUG
	std::cout<<"Debug mode active\n";
	#endif
	// this projection matrix transform the ndc of opengl to the coords system of the size of the window
	
	widthProjection = (static_cast<float>(this->w_width) / 6);
	heightProjection = (static_cast<float>(this->w_height) / 6);

	glm::mat4 projection = glm::ortho(0.0f, widthProjection,
		heightProjection, 0.0f, -1.0f, 1.0f);
	
	camera = new Camera(glm::vec2(widthProjection, heightProjection));
	camera->pos = glm::vec3(-1.0f, -1.0f, 0.0f);

	worldGrid = glm::vec2(widthProjection / tileSize, heightProjection / tileSize);
	

	// this object will help to create a editable tile map in scene
	// this i should extend to have differents tilemaps sections to have greaters levels with
	// the sections conecteds
	// the tiles in the tile map will interact with the player
	tileMap = new TileMap(worldGrid, tileSize);

	
	//glm::mat4 persperctiveMat = glm::perspective()
	shadersPrograms.reserve(10);

	shaderEntities = Shader("./shaders/rendererInstancing/Instancing.vs",
		"./shaders/rendererInstancing/renderer.frag",nullptr,"entities");
	shaderEntities.use().setMat4("projection", projection);
	shaderEntities.use().setInt("texture0", 0);
	shaderEntities.use().setInt("texture1", 1);
	

	// shaderPlayer is not use because of a bug that makes cross gpu memory for the entities renderer with the player renderer
	shaderPlayer = Shader("shaders/player/player.vs",
		"shaders/player/player.frag");
	shaderPlayer.use().setMat4("projection", projection);
	shaderPlayer.use().setInt("texture0", 0);

	shaderDebugQuad = Shader("shaders/renderDebugQuad/debugQuad.vs",
		"shaders/renderDebugQuad/debugQuad.frag");
	shaderDebugQuad.use().setMat4("projection", projection);
	
	shaderGrid = Shader("shaders/grid/gridShader.vs",
		"shaders/grid/gridShader.frag");
	shaderGrid.use().setMat4("projection", projection);
	
	shadersPrograms.emplace_back(shaderEntities);
	shadersPrograms.emplace_back(shaderPlayer);
	shadersPrograms.emplace_back(shaderDebugQuad);
	shadersPrograms.emplace_back(shaderGrid);

	//the differents renderers to draw x or y elements on screen
	spriteRendererPlayer = new SpriteRendererInstanced(shaderPlayer,"Player");
	spriteRendererEntities = new SpriteRendererInstanced(shaderEntities, "Entities");
	spriteRendererInstancedTile = new SpriteRendererInstanced(shaderEntities,"Tiles");

	spriteRendererDebugQuad = new rendererDebugQuad(shaderDebugQuad, "Entities");
	spriteRendererDebugQuadPlayer = new rendererDebugQuad(shaderDebugQuad, "Player");
	rendererGridMap = new RendererGridTileMap(shaderGrid,glm::vec4(1.0f));


	
	tilesChangeColor.reserve(2);
	tilesReturnColor.reserve(2);

	// list of entities in the scene
	entitiesInstanced.reserve(100);
	
	Texture2D textCat;
	textCat.load("resources/textures/cat/TextureAtlas.png", true,0);
	Texture2D text2;
	text2.load("resources/textures/spaceCraft/spaceCraft.png", true,1);

	std::cout<<"End of load texture \n";
	//load two different textures
	
	

	//put the textures in a vector
	textures.reserve(2);
	textures.emplace_back(textCat);
	textures.emplace_back(text2);
	playerTex.reserve(1);
	playerTex.emplace_back(textCat);
	

	
	this->initRenderData();
}



void SandBox::initRenderData() {

	

	
	player = new Player(&playerRenderIndex, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(16.0f), glm::vec4(1.0f), glm::vec2(5.0f,3.0f),
		glm::vec2(-8.0f,-4.0f), (float)textures[0].slot);


	/*Entity2D_Instaciaded entity_in2(&instanceEntity_index, glm::vec3(150.0f, 50.0f, 0.0f), glm::vec2(8.0f),
		glm::vec4(0.1f,0.5f,1.0f,1.0f));
	Entity2D_Instaciaded entity_in3(&instanceEntity_index,glm::vec3(100.0f,100.0f,0.0f),glm::vec2(8.0f),
		glm::vec4(0.5f,0.6f,0.7f,1.0f));*/

	EnemySlime2D* enemy1 = new EnemySlime2D(&instanceEntity_index, (float)textures[0].slot, glm::vec3(50.0f, 10.0f, 0.0f));
	EnemySlime2D* enemy2 = new EnemySlime2D(&instanceEntity_index, (float)textures[0].slot, glm::vec3(100.0f, 50.0f, 0.0f));
	// we put the entities that the player will interact in the entitiesInstanced vector
	entitiesInstanced.emplace_back(enemy1);
	entitiesInstanced.emplace_back(enemy2);



	spriteRendererPlayer->reserveBuffer(1);
	spriteRendererPlayer->addEntity(*player);

	// reserving buffer for the entites to render in scene
	spriteRendererEntities->reserveBuffer(100);

	//filling the buffer with the data of entities to render
	spriteRendererEntities->initFillData(entitiesInstanced);
	
	

	//the renderer for the tiles to render
	spriteRendererInstancedTile->reserveBuffer(400);
	
	//A renderer just for draw quads for debuging and see the rect hitboxes of the entities
	spriteRendererDebugQuad->reserveData(100);
	spriteRendererDebugQuad->initData(entitiesInstanced);
	
	//a renderer to see the hitbox of the player only
	spriteRendererDebugQuadPlayer->reserveData(1);

	//a renderer just to show the grid of the tile map
	rendererGridMap->initBufferData();
	rendererGridMap->fillBufferData(tileSize, tileMap->getWidth(), tileMap->getHeight());
	
}



void SandBox::processInput(float dt) {
	glm::mat4 model(1.0f);

	// camera control
	if (this->keys[GLFW_KEY_RIGHT]) {
		camera->ProcessKeyBoard(RIGHT, dt);
	}
	if (this->keys[GLFW_KEY_LEFT]) {
		camera->ProcessKeyBoard(LEFT, dt);
	}
	if (this->keys[GLFW_KEY_UP])
	{
		camera->ProcessKeyBoard(UP, dt);
	}
	if (this->keys[GLFW_KEY_DOWN])
	{
		camera->ProcessKeyBoard(DOWN, dt);
	}
	// player control
	static float runAcceleration = 240.0f;
	constexpr float runReduce = 450.0f;
	constexpr float gravity = 9.8f;
	if (this->keys[GLFW_KEY_D]) {

		float mult = 1.0f;
		if (player->m_speed.x < 0.0f && player->m_grounded || player->m_speed.x < 0.0f && !this->m_gravityInfluence)
			mult = 10.0f;
		else if (player->m_wallTouch)
			mult = 3.0f;
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
	else if (this->keys[GLFW_KEY_A]) {
		// this for the movement in the negative side of the x axis, and if mult is to change much faster 
		// the vel to the positive to the negative side

		float mult = 1.0f;
		
		// if the player is grounded or the gravity is not affecting and the speed was in the positive side
		// will change drastically to the negative
		if ((player->m_speed.x > 0.0f && player->m_grounded) || (player->m_speed.x > 0.0f && !this->m_gravityInfluence))
			mult = 10.0f;
		// if the player is touching a wall it will change but not that quickly like the firts if statement
		else if (player->m_wallTouch)
			mult = 1.0f;
		// if is on the air it will be much lower
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
		// reduce the vel on the x axis y we don't push the player in the positive or negative side 
		// of the axis
		player->m_pushed = false;
		player->m_speed.x = byPowerMath::approach(player->m_speed.x, 0, runReduce * dt);
	
	}
	if (this->keys[GLFW_KEY_W] && !this->m_gravityInfluence) {

		// moving negative in the y axis if there is not gravity influence

		float mult = 1.0f;
		if (player->m_speed.y > 0.0f)

			mult = 10.0f;

		player->m_speed.y = byPowerMath::approach(player->m_speed.y, -player->m_maxSpeed, runAcceleration * mult * dt);

	
	}
	if (this->keys[GLFW_KEY_S] && !this->m_gravityInfluence) {

		// move positive in the y axis, flying if there is not gravity influence
		float mult = 1.0f;
		if (player->m_speed.y < 0.0f)

			mult = 10.0f;

		player->m_speed.y = byPowerMath::approach(player->m_speed.y, player->m_maxSpeed /2 + player->m_maxSpeed / 4, runAcceleration * mult * dt);
	
	}
	if (this->keys[GLFW_KEY_SPACE] && !this->keyProcessed[GLFW_KEY_SPACE] && this->m_gravityInfluence && player->m_grounded)
	{
		// jump if is grounded 
		this->keyProcessed[GLFW_KEY_SPACE] = true;
		player->m_speed.y = -80;
		player->m_grounded = false;
	}
	else if (this->keys[GLFW_KEY_SPACE] && !this->keyProcessed[GLFW_KEY_SPACE] &&
		this->m_gravityInfluence && player->m_wallTouch && !player->m_grounded)
	{
		// if the player is pushed against a wall and did not jump and is not grounded can do a wall jump
		this->keyProcessed[GLFW_KEY_SPACE] = true;
		player->m_speed.y = -45;
		player->m_speed.x = 0.0f;
		player->m_speed.x = player->m_right ? -120 : 100;
	}
	if (!this->keys[GLFW_KEY_W]  && !this->keys[GLFW_KEY_S] && !this->m_gravityInfluence)
	{
		// if therer is not gravity influence need to reduce his vel in the y axis
		player->m_speed.y = byPowerMath::approach(player->m_speed.y, 0, runReduce * dt);

	}
	else if (this->m_gravityInfluence && !player->m_grounded) {

		// affect the player by the gravity depending if is sliding in a wall or not
		// if have friction with a wall, meaning is pushing against a wall the gravity will 
		// be lower

		bool get_positiveSpeed = player->m_speed.y >= 0.0f;

		float actualGravity = player->m_wallTouch && get_positiveSpeed ? gravity * 3 : gravity * 12;
		actualGravity = !player->m_pushed ? gravity * 15 : actualGravity;

		player->m_speed.y = byPowerMath::approach(player->m_speed.y, player->m_maxSpeed * 2.0f,
			actualGravity * dt);
	}
	// if the player is grounded therer is not gravity affecting him
	else if (this->m_gravityInfluence && player->m_grounded)
	{
		player->m_speed.y = 0.0f;
	}
	
	
	
	player->move(dt);
	detectTileCol(*player);

	
	


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

	// set the zoom of the camera 
	camera->processMouseScroll(yoffset);

}

void SandBox::GetMousePos(glm::vec2 pos)
{	
	// this gets the mouse pos in screen
	this->m_mousePosScreen = glm::vec2(pos.x / 6, pos.y / 6);
	
}

void SandBox::updateMousePos()
{
	//get the global pos of the mouse relative with the camera zoom
	glm::vec3 diff = globalOrigin - camera->pos;
	glm::vec3 mouseGlobalPos = glm::vec3(this->m_mousePosScreen, 0.0f) - diff;
	this->m_mousePosGlobal = glm::vec2(mouseGlobalPos.x, mouseGlobalPos.y) / camera->Zoom;
}

void SandBox::update(float& dt) {
	this->updateMousePos();
	
	player->m_anim(dt);
	spriteRendererPlayer->emptyAllData();
	spriteRendererPlayer->addEntity(*player);
	spriteRendererDebugQuadPlayer->emptyData();
	spriteRendererDebugQuadPlayer->updateData(*player);


	for (int i = 0; i < entitiesInstanced.size(); i++)
	{
		detectEntityCol(*player, *entitiesInstanced[i], dt);
	}

	this->updateEntities(dt);
	
	
	// the next commented line update the render buffer to render tiles
	// it goes throught a matrix to see the tiles that are visible and add to a list
	// to pass to feed the render buffer
	//tileMap->m_updateTilesBufferRenderer(*spriteRendererInstancedTile);

	// this is to animate the player changing his texCoords, i will extend this to make it better 
	// in the entity2D_instanced class
	
	this->handleEntitiesInstances();

	
}

//This update all the entities where the player has no control,npcs or enemies
void SandBox::updateEntities(float& dt) {

	//float vel = dt * velocity / 1.5;
	
	for (int i = 0; i < entitiesInstanced.size(); i++)
	{
		Entity2D_Instaciaded* entity = entitiesInstanced[i];
		glm::vec3 cameraPos = camera->getCameraPos();
		glm::vec2 projectionSizeCamera = camera->getProjectionSize();
		if (entity->m_position.x >= cameraPos.x && entity->m_position.y >= cameraPos.y &&
			entity->m_position.x < cameraPos.x + projectionSizeCamera.x &&
			entity->m_position.y < cameraPos.y + projectionSizeCamera.y)
		{
			entity->m_anim(dt);
			
		}
		
		if (entity->m_position.x >= cameraPos.x - 100 && entity->m_position.y >= cameraPos.y - 100 &&
			entity->m_position.x < cameraPos.x + projectionSizeCamera.x + 100 &&
			entity->m_position.y < cameraPos.y + projectionSizeCamera.y + 100)
		{
			entity->move(dt);
			checkEntitiesTileLimits(*entity,this->m_gravityInfluence,dt);
		}

		
		
	}
	spriteRendererEntities->emptyAllData();
	spriteRendererDebugQuad->emptyData();
	spriteRendererEntities->initFillData(entitiesInstanced);
	spriteRendererDebugQuad->initData(entitiesInstanced);

	
}

static bool output = false;
void SandBox::renderGUI() {

	glm::vec2 gridPos = tileMap->m_getGridPos(glm::vec2(player->m_getEntityRect().pos));

	if (gridPos.y < 0.0f)
		gridPos.y = (int)gridPos.y - 1;
	if (gridPos.x < 0.0f)
		gridPos.x = (int)gridPos.x - 1;
	
	gui.showVec("camera", camera->pos,NULL);
	gui.showVec("Player gridPos", glm::vec3((int)gridPos.x,(int)gridPos.y, 0.0f));
	gui.m_DebugPlayer(*player);



	gui.showVec("globalMousePos", glm::vec3(this->m_mousePosGlobal,0.0f));
	gui.enableTileMapEditing(this->m_enableTileMapEditing, this->m_showGridMap);
	gui.enableBool("gravity influence", this->m_gravityInfluence);
	gui.enableBool("Show entities quads", this->m_showEntitiesQuads);
	
	

}

void SandBox::renderScene() {

	
	if (m_showGridMap)
		rendererGridMap->draw(*camera);

	spriteRendererInstancedTile->draw(*camera, tilesTex);
	spriteRendererEntities->draw(*camera,textures);
	
	if (this->m_showEntitiesQuads)
		spriteRendererDebugQuad->draw(*camera);
	


	spriteRendererPlayer->draw(*camera, textures);
	if (player->m_returnVisibilityHitbox())
		spriteRendererDebugQuadPlayer->draw(*camera);
}

void SandBox::handleEntitiesInstances()
{
	for (int i = 0; i < entitiesInstanced.size(); i++)
	{
		if (entitiesInstanced[i]->m_delete)
			entitiesInstanced.erase(entitiesInstanced.begin() + i);
	}
}

void SandBox::m_resizeCamera(int width, int height)
{
	this->w_width = width;
	this->w_height = height;
	widthProjection = (static_cast<float>(this->w_width) / 6);
	heightProjection = (static_cast<float>(this->w_height) / 6);
	std::cout << "Change camera size\n";
	glm::mat4 projection = glm::ortho(0.0f, widthProjection,
		heightProjection, 0.0f, -1.0f, 1.0f);
	camera->m_projectionSize = glm::vec2(widthProjection, heightProjection);
	//worldGrid = glm::vec2(widthProjection / tileSize, heightProjection / tileSize);
	//tileMap->m_updateTileMap(worldGrid, tileSize);

	for (int i = 0; i < shadersPrograms.size(); i++)
	{
		shadersPrograms[i].use().setMat4("projection", projection);
	}
}