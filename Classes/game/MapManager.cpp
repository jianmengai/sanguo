#include "MapManager.h"
#include "AutoFindPath.h"
#include "GameObjectManager.h"

static std::map<TileMapLayerType, std::string> s_tileMapLayerTypeToString = {
	{ TileMapLayerType::BackgroundLayer, "backgroundLayer" },
	{ TileMapLayerType::GameObjcetLayer, "gameObjectLayer" },
};

MapManager* MapManager::getInstance()
{
	static MapManager mapManagerInstance;
	return &mapManagerInstance;
}

MapManager::MapManager()
{
	m_tiledMap = nullptr;
	m_gameObjectLayer = nullptr;
	m_mapScale = 0.3f;
}

MapManager::~MapManager()
{

}



bool MapManager::init(cocos2d::Layer* parentLayer, const std::string& mapFileName)
{
	m_tiledMap = cocos2d::experimental::TMXTiledMap::create(mapFileName);
	if (m_tiledMap == nullptr)
	{
		return false;
	}
	m_gameObjectLayer = m_tiledMap->getLayer(s_tileMapLayerTypeToString[TileMapLayerType::GameObjcetLayer]);
	if (m_gameObjectLayer == nullptr)
	{
		return false;
	}
	m_tiledMap->setScale(m_mapScale);
	m_tiledMap->setPosition(cocos2d::Vec2(-1700,-1200));

	parentLayer->addChild(m_tiledMap);

	m_drawNode = cocos2d::DrawNode::create();

	m_gameObjectLayer->addChild(m_drawNode);

	m_mapSize = m_tiledMap->getMapSize();
	m_tileSize = m_tiledMap->getTileSize();

	initTileNodeTable();
	AutoFindPath::initTileNodeTable(m_tileNodeTable);
	
	drawTileTable();

	m_mapContentSize = m_tiledMap->getContentSize();
	m_clientWinSize = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::log("map content size, width:%0.1f, height:%0.1f", m_mapContentSize.width, m_mapContentSize.height);
	cocos2d::log("tile size, width:%0.1f, height:%0.1f", m_tiledMap->getTileSize().width, m_tiledMap->getTileSize().height);
	cocos2d::log("map size, width:%0.1f, height:%0.1f", m_tiledMap->getMapSize().width, m_tiledMap->getMapSize().height);
	cocos2d::log("anchor point, x:%0.1f, y;%0.1f", m_tiledMap->getAnchorPoint().x, m_tiledMap->getAnchorPoint().y);


	return true;
}

void MapManager::update(float dt)
{
	static int count = 0;
	if (count >= 120)
	{
		drawTileTable();
		count = 0;
	}
	++count;
}

void MapManager::setMapScale(float scale)
{
	m_mapScale = scale;
	m_tiledMap->setScale(m_mapScale);
}

float MapManager::getMapScale()
{
	return m_mapScale;
}

void MapManager::initTileNodeTable()
{
	//static int gid = 1;
	//auto mapSize = m_tiledMap->getMapSize();
	m_tileNodeTable.resize((std::size_t)m_mapSize.height);

	//auto tileSize = m_tiledMap->getTileSize();
	auto gameObjectLayer = m_tiledMap->getLayer(s_tileMapLayerTypeToString[TileMapLayerType::GameObjcetLayer]);

	cocos2d::log("\n-------gameoject layer-------");
	cocos2d::log("	tilesize: width=%0.1f, height=%0.1f", gameObjectLayer->getMapTileSize().width, gameObjectLayer->getMapTileSize().height);
	cocos2d::log("	contentsize: width=%0.1f, height=%0.1f", gameObjectLayer->getContentSize().width, gameObjectLayer->getContentSize().height);
	cocos2d::log("	layersize: width=%0.1f, height=%0.1f\n", gameObjectLayer->getLayerSize().width, gameObjectLayer->getLayerSize().height);

	auto totalHeight = m_tileSize.height * m_mapSize.height;
	for (int rowIndex = 0; rowIndex < (int)m_mapSize.height; rowIndex++)
	{
		m_tileNodeTable[rowIndex].resize((int)m_mapSize.width);
		for (int columnIndex = 0; columnIndex < (int)m_mapSize.width; columnIndex++)
		{
			m_tileNodeTable[rowIndex][columnIndex] = new TileNode;

			m_tileNodeTable[rowIndex][columnIndex]->gid = gameObjectLayer->getTileGIDAt(cocos2d::Vec2(columnIndex, rowIndex));
			if (m_tileNodeTable[rowIndex][columnIndex]->gid != 0)
			{
				m_tileNodeTable[rowIndex][columnIndex]->occupy = 1;
			}

			m_tileNodeTable[rowIndex][columnIndex]->position.x = columnIndex * m_tileSize.width + m_tileSize.width / 2.0;
			m_tileNodeTable[rowIndex][columnIndex]->position.y = (m_mapSize.height - rowIndex - 1) * m_tileSize.height + m_tileSize.height / 2.0;
			m_tileNodeTable[rowIndex][columnIndex]->rowIndex = rowIndex;
			m_tileNodeTable[rowIndex][columnIndex]->columnIndex = columnIndex;

			m_tileNodeTable[rowIndex][columnIndex]->parent = nullptr;
			m_tileNodeTable[rowIndex][columnIndex]->isVisit = false;
		}
	}
}

void MapManager::drawTileTable()
{
	cocos2d::Color4F color = cocos2d::Color4F(164.0f / 255.0f, 72.0f / 255.0f, 192.0f / 255.0f, 1.0f);
	auto totalHeight = m_tileSize.height * m_mapSize.height;
	for (int rowIndex = 0; rowIndex < (int)m_mapSize.height; rowIndex++)
	{
		for (int columnIndex = 0; columnIndex < (int)m_mapSize.width; columnIndex++)
		{
			auto originPoint = cocos2d::Vec2(columnIndex * m_tileSize.width, totalHeight - rowIndex * m_tileSize.height);
			auto destPoint = cocos2d::Vec2((columnIndex + 1) * m_tileSize.width, totalHeight - (rowIndex + 1) * m_tileSize.height);
			if (m_tileNodeTable[rowIndex][columnIndex]->occupy == 1)
			{
				m_drawNode->drawSolidRect(originPoint, destPoint, color);
			}
			else
			{
				m_drawNode->drawRect(originPoint, destPoint, color);
			}
			
		}
	}
}

void MapManager::setPosition(cocos2d::Vec2& position, bool isDelta)
{
	cocos2d::Vec2 newPosition = position * m_mapScale;
	if (isDelta)
	{
		cocos2d::Vec2 pos = m_tiledMap->getPosition();
		//cocos2d::log("old map postion, x:%0.1f, y:%0.1f", pos.x, pos.y);
		pos.negate();
		
		newPosition = pos - newPosition;
	}
	if ((newPosition.x + m_clientWinSize.width) >= m_mapContentSize.width * m_mapScale)
	{
		newPosition.x = m_mapContentSize.width * m_mapScale - m_clientWinSize.width;
	}
	if (newPosition.x < 0.0f)
	{
		newPosition.x = 0.0f;
	}
	if ((newPosition.y + m_clientWinSize.height) >= m_mapContentSize.height * m_mapScale)
	{
		newPosition.y = m_mapContentSize.height * m_mapScale - m_clientWinSize.height;
	}
	if (newPosition.y < 0.0f)
	{
		newPosition.y = 0.0f;
	}
	newPosition.negate();
	m_tiledMap->setPosition(newPosition);
	//cocos2d::log("set map postion, x:%0.1f, y:%0.1f", newPosition.x, newPosition.y);
}


void MapManager::addChildToGameObjectLayer(cocos2d::Sprite* unit, int zOrder)
{
	m_gameObjectLayer->addChild(unit, zOrder);
	cocos2d::log("child scale:%0.1f", unit->getScale());
}

cocos2d::Vec2 MapManager::getPosition()
{
	return m_tiledMap->getPosition();
}

cocos2d::Size MapManager::getTileSize()
{
	return m_tiledMap->getTileSize();
}

cocos2d::Size MapManager::getMapSize()
{
	return m_mapSize;
}

cocos2d::Size MapManager::getContentSize()
{
	return m_mapContentSize;
}

cocos2d::Vec2 MapManager::toMapPos(const cocos2d::Vec2& pos)
{
	auto mapPos = m_tiledMap->getPosition();
	mapPos.negate();
	cocos2d::log("map pos:[%0.1f, %0.1f]", mapPos.x, mapPos.y);
	mapPos += pos;
	cocos2d::log("touch pos:[%0.1f, %0.1f]", pos.x, pos.y);
	mapPos = mapPos / m_mapScale;
	cocos2d::log("new map pos:[%0.1f, %0.1f]", mapPos.x, mapPos.y);
	return mapPos;
}

cocos2d::Vec2 MapManager::toTileRowCol(const cocos2d::Vec2& pos)
{
	auto tileSize = m_tiledMap->getTileSize();
	int col = static_cast<int>(pos.x / tileSize.width);
	int row = static_cast<int>(pos.y / tileSize.height);
	if (col >= tileSize.width)
	{
		col = 0;
	}
	row = m_mapSize.height - row - 1;
	if (row < 0)
	{
		row = 0;
	}
	return cocos2d::Vec2(row, col);
}

cocos2d::Vec2 MapManager::tileRowColToPos(const int row, const int col)
{
	if (row >= m_mapSize.height || col >= m_mapSize.width)
	{
		return cocos2d::Vec2::ZERO;
	}
	if (row < 0 || col < 0)
	{
		return cocos2d::Vec2::ZERO;
	}
	return m_tileNodeTable[row][col]->position;
}

void MapManager::setOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize)
{
	auto rowCol = toTileRowCol(pos);
	if ((rowCol.x >= m_mapSize.height) || (rowCol.y >= m_mapSize.width))
	{
		return ;
	}

	int dtCol = contentSize.width / m_tileSize.width;
	int dtRow = contentSize.height / 2.0 / m_tileSize.height;
	cocos2d::Vec2 endRowCol = rowCol;
	endRowCol.x -= dtRow;
	endRowCol.y += dtCol;
	if (endRowCol.x < 0)
	{
		return ;
	}

	int startRow = std::min(rowCol.x, endRowCol.x);
	int endRow = std::max(rowCol.x, endRowCol.x);

	int startCol = std::min(rowCol.y, endRowCol.y);
	int endCol = std::max(rowCol.y, endRowCol.y);

	for (int i = startRow; i <= endRow; ++i)
	{
		for (int j = startCol; j <= endCol; ++j)
		{
			cocos2d::log("map row:%d, col:%d, set row:%d, col:%d, set occupy", (int)rowCol.x, (int)rowCol.y, i, j);
			m_tileNodeTable[i][j]->occupy = 1;
		}
	}
}

bool MapManager::checkOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize)
{
	auto rowCol = toTileRowCol(pos);
	if ((rowCol.x >= m_mapSize.height) || (rowCol.y >= m_mapSize.width))
	{
		return false;
	}

	int dtCol = contentSize.width / m_tileSize.width;
	int dtRow = contentSize.height / 2.0 / m_tileSize.height;
	cocos2d::Vec2 endRowCol = rowCol;
	endRowCol.x -= dtRow;
	endRowCol.y += dtCol;
	if (endRowCol.x < 0)
	{
		return false;
	}

	int startRow = std::min(rowCol.x, endRowCol.x);
	int endRow = std::max(rowCol.x, endRowCol.x);

	int startCol = std::min(rowCol.y, endRowCol.y);
	int endCol = std::max(rowCol.y, endRowCol.y);

	for (int i = startRow; i <= endRow; ++i)
	{
		for (int j = startCol; j <= endCol; ++j)
		{
			if (m_tileNodeTable[i][j]->occupy == 1)
			{
				cocos2d::log("row:%d, col:%d occupied", i, j);
				return false;
			}
		}
	}

	return true;
}

bool MapManager::checkOccupy(const int row, const int col)
{
	if (row >= m_mapSize.height || col >= m_mapSize.width)
	{
		return false;
	}
	if (row < 0 || col < 0)
	{
		return false;
	}
	return m_tileNodeTable[row][col]->occupy != 1;
}

TileNode* MapManager::getTileNode(const cocos2d::Vec2& pos)
{
	cocos2d::Vec2 rowCol = toTileRowCol(pos);
	return m_tileNodeTable[rowCol.x][rowCol.y];
}

TileNode* MapManager::getTileNode(int row, int col)
{
	if (row >= m_mapSize.height || col >= m_mapSize.width)
	{
		return nullptr;
	}

	return m_tileNodeTable[row][col];
}

void MapManager::updateTileNodeTable()
{
	//实时刷新各个格子的占用情况，以及格子上object信息
	GameObjectMap& gameObjects = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto& it : gameObjects)
	{
		GameObject* gameObject = it.second;
	}
}