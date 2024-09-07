#include"TileMap.h"
#include"debugLog/debugLog.h"

TileMap::TileMap(glm::vec2 mapSize, int tileSize)
	: m_mapSize(mapSize), m_tileSize(tileSize),m_tileRenderIndex(0)
{
	this->m_levelTileMap.reserve((int)this->m_mapSize.y);
	this->m_tilesToRender.reserve((int)this->m_mapSize.y * (int)this->m_mapSize.x);

	

	for (int y = 0; y < (int)m_mapSize.y; y++)
	{
		std::vector<Tile> columns;
		columns.reserve((int)this->m_mapSize.x);
		for (int x = 0; x <= (int)m_mapSize.x; x++)
		{
			int meanTimeRenderIndex = -1;
			columns.emplace_back(Tile(&meanTimeRenderIndex, glm::vec3(x * this->m_tileSize, y * this->m_tileSize, 0.0f),
				glm::vec2(this->m_tileSize)));
			
		}
		this->m_levelTileMap.emplace_back(columns);
	}

}

TileMap::~TileMap(){ }

Tile* TileMap::m_GetTile(glm::vec3 worldPos)
{
	float x = worldPos.x / this->m_tileSize;
	float y = worldPos.y / this->m_tileSize;


	Tile* tile = nullptr;

	if (x >= 0 && x <= (int)this->m_mapSize.x && y >= 0 && y < (int)this->m_mapSize.y) {
		int x_int = (int)x;
		int y_int = (int)y;
		tile = &this->m_levelTileMap[y_int][x_int];
	}

	return tile;
}

Tile* TileMap::m_GetGridTile(glm::vec2 gridPos)
{
	Tile* tile = nullptr;

	if (gridPos.x >= 0 && gridPos.x <= (int)this->m_mapSize.x && gridPos.y >= 0 && gridPos.y < (int)this->m_mapSize.y)
	{
		tile = &this->m_levelTileMap[gridPos.y][gridPos.x];
	}

	return tile;
}

glm::vec2 TileMap::m_getGridPos(glm::vec2 worldPos)
{
	int x = worldPos.x / this->m_tileSize;
	int y = worldPos.y / this->m_tileSize;

	return glm::vec2(x, y);
}

void TileMap::m_addTileToRender(glm::vec3 wordlPos)
{
	Tile* tile = this->m_GetTile(wordlPos);

	if (tile && !tile->m_isVisible())
	{
		tile->m_setVisible(true, this->m_tileRenderIndex);
		this->m_tilesToRender.emplace_back(*tile);


		this->m_tileRenderIndex++;


	}
}

void TileMap::m_deleteTileFromRender(glm::vec3 worldPos)
{
	Tile* tile = this->m_GetTile(worldPos);



	if (tile && tile->m_isVisible())
	{
		
		int tileIndex = tile->m_returnRenderIndex();
		int lastIndex = m_tilesToRender[m_tilesToRender.size() - 1].m_returnRenderIndex();
		tile->m_setVisible(false, this->m_tileRenderIndex);
		
		

		if (this->m_tilesToRender.size() > 0)
			this->m_tilesToRender.erase(this->m_tilesToRender.begin() + tileIndex);
		

		if (tileIndex < lastIndex)
		{
			for (int i = tileIndex; i < lastIndex; i++)
			{
				glm::vec3 tilePos = this->m_tilesToRender[i].m_position;
				Tile* tileToChange = this->m_GetTile(tilePos);
				tileToChange->m_setRenderIndex(i);
				this->m_tilesToRender[i] = *(tileToChange);
			}
		}

		this->m_tileRenderIndex--;
		

	}
	
}

void TileMap::m_addTile(glm::vec3 worldPos)
{
	Tile* tile = this->m_GetTile(worldPos);

	if (tile && !tile->m_isVisible())
	{
		tile->m_setVisible(true);

	}
}

void TileMap::m_removeTile(glm::vec3 worldPos)
{
	Tile* tile = this->m_GetTile(worldPos);

	if (tile && tile->m_isVisible())
	{
		tile->m_setVisible(false);
	}
}

void TileMap::m_updateTilesBufferRenderer(SpriteRendererInstanced& tileRenderer)
{
	std::vector<Entity2D_Instaciaded> tiles;
	tiles.reserve(this->m_levelTileMap.size() * this->m_levelTileMap[0].size());
	for (int y = 0; y < this->m_levelTileMap.size(); y++)
	{
		for (int x = 0; x < this->m_levelTileMap[y].size(); x++)
		{
			if (this->m_levelTileMap[y][x].m_isVisible())
				tiles.emplace_back(this->m_levelTileMap[y][x]);
		}
	}
	if (tiles.size() > 0)
		tileRenderer.initFillData(tiles);
	else
		tileRenderer.emptyAllData();
}

void TileMap::m_updateTileRenderBuffer(SpriteRendererInstanced& tileRenderer)
{
	if (this->m_tilesToRender.size() > 0)
		tileRenderer.initFillData(this->m_tilesToRender);
	else
		tileRenderer.emptyAllData();
}

float TileMap::getWidth()
{
	return (int)this->m_mapSize.x;
}

float TileMap::getHeight()
{
	return (int)this->m_mapSize.y;
}
