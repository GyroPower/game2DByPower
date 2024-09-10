#include<glad/gl.h>
#include"sandBox.h"
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<GLFW/glfw3.h>
#include"GUI/Gui.h"
#include"Texture/texture.h"
#include<vector>
#include<stb_image/stb_image.h>
#include"entity/entity.h"
#include"entity/player/player.h"
#include"Spriterenderer/spriteRenderer.h"
#include"Spriterenderer/rendererDebugQuad/rendererDebugQuad.h"
#include"Spriterenderer/renderTileMap/rendererGridTileMap.h"
#include"debugLog/debugLog.h"
#include"TileMap/TileMap.h"
#include"mathFuncs/mathFuncs.h"
#include"entity/colFunctions/colFunctions.h"


static Camera camera;
static GUI gui;
static Shader shaderP;
static Shader shaderIntancing;
static Shader shaderDebugQuad;
static Shader shaderGrid;
static Shader shaderPlayer;
static SpriteRendererInstanced* spriteRendererInstanced;
static SpriteRendererInstanced* spriteRendererInstancedPlayer;
static rendererDebugQuad* spriteRendererDebugQuad;
static rendererDebugQuad* spriteRendererDebugQuadPlayer;
static RendererGridTileMap* rendererGridMap;
static SpriteRendererInstanced* spriteRendererInstancedTile;
static TileMap* tileMap;

static std::vector<Texture2D> textures;
static float velocity = 150.0f;
static float animTime = 0.0f;
static std::vector<Entity2D_Instaciaded> entitiesInstanced;
static Player* player;
static glm::vec2 worldGrid(0.0f, 0.0f);
static glm::vec3 globalOrigin(0.0f);
static std::vector<Texture2D> playerTex;
static std::vector<Texture2D> tilesTex;
static std::vector<Tile> tilesChangeColor;
static std::vector<Tile> tilesReturnColor;

static int initialSizeChange = 0;
static int initialSizeReturn = 0;

static int instanceTileRender_index = 0;
static int instanceEntity_index = 0;
static int playerRenderIndex = 0;
static float scale = 1.0f;
static float widthProjection = 0.0f;
static float heightProjection = 0.0f;
static float tileSize = 8.0f;


static glm::vec2 getPlayerGridPos(glm::vec2 worlPos)
{
	return glm::vec2(worlPos.x / tileSize, worlPos.y / tileSize);
}


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
				
				// check collisions
				if (rect_collision(entity.m_getEntityRect(), tile->m_getEntityRect()))
				{
					entity.m_col = true;

					// check if came from the bottom

					Rect entityRect = entity.m_getEntityRect();
					Rect tileRect = tile->m_getEntityRect();
					if ((entity.m_getEntityRect().previusPos.y + entity.m_getEntityRect().size.y <= tile->m_getEntityRect().pos.y &&
						entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y > tile->m_getEntityRect().pos.y) ||
						(entity.m_getEntityRect().previusPos.y + entity.m_getEntityRect().size.y < tile->m_getEntityRect().pos.y + (tile->m_getEntityRect().size.y / 2) &&
							entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y > tile->m_getEntityRect().pos.y))
					{
						entity.m_color = glm::vec4(0.5f, 1.0f, 1.0f, 1.0f);

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
						entity.m_color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
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
						entity.m_color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
						entity.m_wallTouch = true;
					}

				}
				else
					entity.m_col = false;
				// if not collision, see if the entity is still grounded
				if (y == gridPos.y + 2 && (x == gridPos.x || x == gridPos.x + 1) && entity.m_getEntityRect().pos.y + entity.m_getEntityRect().size.y + 0.1 <
					tile->m_getEntityRect().pos.y)
				{
					entity.m_grounded = false;
					entity.m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				}
				else if (y == gridPos.y - 1 && entity.m_getEntityRect().pos.y - 0.1f > tile->m_getEntityRect().pos.y + tile->m_getEntityRect().size.y)
					entity.m_top = false;
				
				if (entity.m_wallTouch && !entity.m_pushed)
				{
					entity.m_wallTouch = false;
					entity.m_left = false;
					entity.m_right = false;
					entity.m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
					entity.m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
					entity.m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
	delete spriteRendererInstanced;
	delete spriteRendererDebugQuadPlayer;
	delete spriteRendererInstancedPlayer;
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
	

	camera.pos = glm::vec3(-1.0f, -1.0f, 0.0f);

	worldGrid = glm::vec2(widthProjection / tileSize, heightProjection / tileSize);
	

	// this object will help to create a editable tile map in scene
	// this i should extend to have differents tilemaps sections to have greaters levels with
	// the sections conecteds
	// the tiles in the tile map will interact with the player
	tileMap = new TileMap(worldGrid, tileSize);

	
	//glm::mat4 persperctiveMat = glm::perspective()
	

	shaderIntancing = Shader("../shaders/rendererInstancing/Instancing.vs",
		"../shaders/rendererInstancing/renderer.frag");
	shaderIntancing.use().setMat4("projection", projection);
	shaderIntancing.use().setInt("texture0", 0);
	shaderIntancing.use().setInt("texture1", 1);
	
	// shaderPlayer is not use because of a bug that makes cross gpu memory for the entities renderer with the player renderer
	shaderPlayer = Shader("../shaders/player/player.vs",
		"../shaders/player/player.frag");
	shaderPlayer.use().setMat4("projection", projection);
	shaderPlayer.use().setInt("texture0", 0);

	shaderDebugQuad = Shader("../shaders/renderDebugQuad/debugQuad.vertex",
		"../shaders/renderDebugQuad/debugQuad.fragment");
	shaderDebugQuad.use().setMat4("projection", projection);
	
	shaderGrid = Shader("../shaders/grid/gridShader.vs",
		"../shaders/grid/gridShader.frag");
	shaderGrid.use().setMat4("projection", projection);


	//the differents renderers to draw x or y elements on screen
	spriteRendererInstanced = new SpriteRendererInstanced(shaderIntancing,"Entities");
	spriteRendererInstancedPlayer = new SpriteRendererInstanced(shaderPlayer,"Player");
	spriteRendererInstancedTile = new SpriteRendererInstanced(shaderIntancing,"Tiles");

	spriteRendererDebugQuad = new rendererDebugQuad(shaderDebugQuad, "Entities");
	spriteRendererDebugQuadPlayer = new rendererDebugQuad(shaderDebugQuad, "Player");
	rendererGridMap = new RendererGridTileMap(shaderGrid,glm::vec4(1.0f));


	
	tilesChangeColor.reserve(2);
	tilesReturnColor.reserve(2);

	// list of entities in the scene
	entitiesInstanced.reserve(100);
	
	//load two different textures
	Texture2D textCat;
	textCat.load("../resources/textures/cat/TextureAtlas.png", true,0);
	Texture2D text2;
	text2.load("../resources/textures/spaceCraft/spaceCraft.png", true,1);

	//put the textures in a vector
	textures.reserve(2);
	textures.emplace_back(textCat);
	textures.emplace_back(text2);
	playerTex.reserve(1);
	playerTex.emplace_back(textCat);
	

	
	this->initRenderData();
}



void SandBox::initRenderData() {

	

	
	player = new Player(&instanceEntity_index, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(16.0f), glm::vec4(1.0f), glm::vec2(5.0f,3.0f),
		glm::vec2(-8.0f,-4.0f), (float)textures[0].slot);
	Entity2D_Instaciaded entity_in2(&instanceEntity_index, glm::vec3(150.0f, 50.0f, 0.0f), glm::vec2(8.0f),
		glm::vec4(0.1f,0.5f,1.0f,1.0f));
	Entity2D_Instaciaded entity_in3(&instanceEntity_index,glm::vec3(100.0f,100.0f,0.0f),glm::vec2(8.0f),
		glm::vec4(0.5f,0.6f,0.7f,1.0f));

	
	// we put the entities that the player will interact in the entitiesInstanced vector
	entitiesInstanced.emplace_back(entity_in2);
	entitiesInstanced.emplace_back(entity_in3);

	// reserving buffer for the entites to render in scene
	spriteRendererInstanced->reserveBuffer(100);

	//filling the buffer with the data of entities to render
	spriteRendererInstanced->addEntity(*player);
	spriteRendererInstanced->initFillData(entitiesInstanced);
	
	//The next two lines of code are commented, I would like to use a renderer just for the player
	// but therer is a bug, when i updated data just for the player, it does too for the renderer of 
	// the entities, and i don't know why, but for know i would leave this way
	/*spriteRendererInstancedPlayer->reserveBuffer(1);
	spriteRendererInstancedPlayer->addEntity(*player);*/

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
	// player control
	static float runAcceleration = 240.0f;
	constexpr float runReduce = 280.0f;
	constexpr float gravity = 9.8f;
	if (this->keys[GLFW_KEY_D]) {

		float mult = 1.0f;
		if (player->m_speed.x < 0.0f && player->m_grounded || player->m_speed.x < 0.0f && !this->m_gravityInfluence)
			mult = 100.0f;
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
		if (player->m_speed.x > 0.0f && player->m_grounded || player->m_speed.x > 0.0f && !this->m_gravityInfluence)
			mult = 100.0f;
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

			mult = 100.0f;

		player->m_speed.y = byPowerMath::approach(player->m_speed.y, -player->m_maxSpeed, runAcceleration * mult * dt);

	
	}
	if (this->keys[GLFW_KEY_S] && !this->m_gravityInfluence) {

		// move positive in the y axis, flying if there is not gravity influence
		float mult = 1.0f;
		if (player->m_speed.y < 0.0f)

			mult = 100.0f;

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

	
	
	spriteRendererInstanced->updateEntity(*player);
	

	if (player->m_returnVisibilityHitbox())
		spriteRendererDebugQuadPlayer->updateData(*player);
	else if (!player->m_returnVisibilityHitbox())
		spriteRendererDebugQuadPlayer->emptyData();

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
	camera.processMouseScroll(yoffset);

}

void SandBox::GetMousePos(glm::vec2 pos)
{	
	// this gets the mouse pos in screen
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
	
	
	// the next commented line update the render buffer to render tiles
	// it goes throught a matrix to see the tiles that are visible and add to a list
	// to pass to feed the render buffer
	//tileMap->m_updateTilesBufferRenderer(*spriteRendererInstancedTile);

	// this is to animate the player changing his texCoords, i will extend this to make it better 
	// in the entity2D_instanced class
	player->m_anim(dt);
	spriteRendererInstanced->updateEntity(*player);

	
}

//This update all the entities where the player has no control,npcs or enemies
void SandBox::updateEntities(float& dt) {

	float vel = dt * velocity / 1.5;
	
	

	
		
	
}

void SandBox::renderGUI() {

	//This is where all the gui elements are called to created and render
	glm::vec2 gridPos = tileMap->m_getGridPos(glm::vec2(player->m_getEntityRect().pos));

	if (gridPos.y < 0.0f)
		gridPos.y = (int)gridPos.y - 1;
	if (gridPos.x < 0.0f)
		gridPos.x = (int)gridPos.x - 1;
	//gui.useSliderForVec2("Vec for change y and x texCoords", entities[0].maxTexCoords);
	gui.showVec("camera", camera.pos,NULL);
	gui.showVec("Player gridPos", glm::vec3((int)gridPos.x,(int)gridPos.y, 0.0f));
	
	gui.m_DebugPlayer(*player);


	gui.showVec("globalMousePos", glm::vec3(this->m_mousePosGlobal,0.0f));
	gui.enableTileMapEditing(this->m_enableTileMapEditing, this->m_showGridMap);
	gui.enableBool("gravity influence", this->m_gravityInfluence);
	gui.enableBool("Show entities quads", this->m_showEntitiesQuads);
	
	
	//gui.enableBool("showGrid", this->m_showGridMap);
	//gui.useSliderForFloat("scale", scale);
	
	

}

void SandBox::renderScene() {

	//spriteRenderer->draw(vertices, indices, camera, textures);
	
	if (m_showGridMap)
		rendererGridMap->draw(camera);

	spriteRendererInstancedTile->draw(camera, tilesTex);
	spriteRendererInstanced->draw(camera,textures);
	if (player->m_returnVisibilityHitbox())
		spriteRendererDebugQuadPlayer->draw(camera);
	if (this->m_showEntitiesQuads)
	{
		
		spriteRendererDebugQuad->draw(camera);

	}
}

