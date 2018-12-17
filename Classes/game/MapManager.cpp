#include "MapManager.h"
#include "AutoFindPath.h"
#include "GameObjectManager.h"
#include "WarFogLayer.h"

static std::map<TileMapLayerType, std::string> s_tileMapLayerTypeToString = {
	{ TileMapLayerType::BackgroundLayer, "backgroundLayer" },
	{ TileMapLayerType::GameObjcetLayer, "gameObjectLayer" },
	{ TileMapLayerType::ResourceLayer, "resourceLayer"},
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
	//m_tiledMap->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	m_tiledMap->setScale(m_mapScale);
	//m_tiledMap->setPosition(cocos2d::Vec2(-1700,-1200));

	parentLayer->addChild(m_tiledMap);

	m_drawNode = cocos2d::DrawNode::create();

	m_gameObjectLayer->addChild(m_drawNode);

	m_mapSize = m_tiledMap->getMapSize();
	m_tileSize = m_tiledMap->getTileSize();

	
	m_tileMapOriginCoor = cocos2d::Vec2(m_mapSize.width / 2.0 * m_tileSize.width, m_mapSize.height * m_tileSize.height - m_tileSize.height / 2.0);

	initTileNodeTable();

	if (!initBasePosition())
	{
		return false;
	}

	AutoFindPath::initTileNodeTable(m_tileNodeTable);
	
	//drawTileTable();

	m_mapContentSize = m_tiledMap->getContentSize();
	m_clientWinSize = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::log("map content size, width:%0.1f, height:%0.1f", m_mapContentSize.width, m_mapContentSize.height);
	cocos2d::log("tile size, width:%0.1f, height:%0.1f", m_tiledMap->getTileSize().width, m_tiledMap->getTileSize().height);
	cocos2d::log("map size, width:%0.1f, height:%0.1f", m_tiledMap->getMapSize().width, m_tiledMap->getMapSize().height);
	cocos2d::log("position:%0.1f, %0.1f", m_tiledMap->getPosition().x, m_tiledMap->getPosition().y);
	cocos2d::log("anchor point, x:%0.1f, y;%0.1f", m_tiledMap->getAnchorPoint().x, m_tiledMap->getAnchorPoint().y);


	return true;
}

void MapManager::update(float dt)
{
	static int count = 0;
	static int count2 = 0;
	if (count >= 120)
	{
		//drawTileTable();
		count = 0;
	}
	//if (count2 >= 30)
	{
		updateOccupy();
		count2 = 0;
	}
	++count;
	++count2;
}

bool MapManager::initBasePosition()
{
	auto resourceLayer = m_tiledMap->getObjectGroup(s_tileMapLayerTypeToString[TileMapLayerType::ResourceLayer]);
	if (resourceLayer == nullptr)
	{
		return false;
	}
	
	std::stringstream ss;
	ss << "npcMainTown";
	std::string baseName = ss.str();
	ss.str("");
	ss << "npcBarrack";
	std::string barrackName = ss.str();
	auto baseNameValueMap = resourceLayer->getObject(baseName);
	if (baseNameValueMap.empty())
	{
		return false;
	}
	auto barrackValueMap = resourceLayer->getObject(barrackName);
		
	BasePosition basePosition;
	basePosition.basePosition = getObjectPosition(baseNameValueMap, baseName);
	basePosition.barrackPosition = getObjectPosition(barrackValueMap, barrackName);
	for (int i = 1; i <= 5; ++i)
	{
		ss.str("");
		ss << "npcArchor" << i;
		std::string archerTowerName = ss.str();
		auto archerTowerValueMap = resourceLayer->getObject(archerTowerName);
		if (archerTowerValueMap.empty())
		{
			break;
		}
		auto archerTowerPostion = getObjectPosition(archerTowerValueMap, archerTowerName);
		basePosition.archerTowerPositions.push_back(archerTowerPostion);
	}
		
	m_basePositions = basePosition;

	ss.str("");
	ss << "playerMainTown";
	std::string playerInitPositionName = ss.str();
	auto playerInitPosValueMap = resourceLayer->getObject(playerInitPositionName);
	m_playerInitPosition = getObjectPosition(playerInitPosValueMap, playerInitPositionName);

	ss.str("");
	ss << "playerScout";
	std::string playerScoutPositionName = ss.str();
	auto scoutPosValueMap = resourceLayer->getObject(playerScoutPositionName);
	m_playerScoutPosition = getObjectPosition(scoutPosValueMap, playerScoutPositionName);
	

	return true;
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

BasePosition& MapManager::getBasePosition()
{
	// TODO: 在此处插入 return 语句
	return m_basePositions;
}

cocos2d::Vec2 & MapManager::getPlayerInitPosition()
{
	// TODO: 在此处插入 return 语句
	return m_playerInitPosition;
}

cocos2d::Vec2 & MapManager::getPlayerScoutPosition()
{
	// TODO: 在此处插入 return 语句
	return m_playerScoutPosition;
}


void MapManager::initTileNodeTable()
{
	/*
	m_tileNodeTable.resize((std::size_t)m_mapSize.height);
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
	*/

	auto mapSize = m_tiledMap->getMapSize();
	m_tileNodeTable.resize((int)mapSize.height);

	auto tileSize = m_tiledMap->getTileSize();

	auto halfTileSize = tileSize / 2.0;

	auto gameObjectLayer = m_tiledMap->getLayer(s_tileMapLayerTypeToString[TileMapLayerType::GameObjcetLayer]);

	for (int rowIndex = 0; rowIndex < (int)mapSize.height; ++rowIndex)
	{
		m_tileNodeTable[rowIndex].resize((int)mapSize.width);
		for (int columnIndex = 0; columnIndex < (int)mapSize.height; ++columnIndex)
		{
			m_tileNodeTable[rowIndex][columnIndex] = new TileNode;

			m_tileNodeTable[rowIndex][columnIndex]->gid = gameObjectLayer->getTileGIDAt(cocos2d::Vec2(columnIndex, rowIndex));
			if (m_tileNodeTable[rowIndex][columnIndex]->gid != 0)
			{
				m_tileNodeTable[rowIndex][columnIndex]->occupy = OccupyType::InValid;
			}
			/*cocos2d::Vec2 positionInTileMap;
			positionInTileMap.x = (((float)columnIndex - (float)rowIndex) / 2.0f + mapSize.width / 2.0f) * tileSize.width;
			positionInTileMap.y = (mapSize.height - ((float)columnIndex + (float)rowIndex) / 2.0f) *  tileSize.height;
			m_tileNodeTable[rowIndex][columnIndex]->position = positionInTileMap;*/

			m_tileNodeTable[rowIndex][columnIndex]->position.x = m_tileMapOriginCoor.x + columnIndex * halfTileSize.width - rowIndex * halfTileSize.width;//(((float)columnIndex - (float)rowIndex) / 2.0f + mapSize.width / 2.0f) * tileSize.width;
			m_tileNodeTable[rowIndex][columnIndex]->position.y = m_tileMapOriginCoor.y - columnIndex * halfTileSize.height - rowIndex * halfTileSize.height;//(mapSize.height - ((float)columnIndex + (float)rowIndex) / 2.0f - 0.5f) *  tileSize.height;;


			m_tileNodeTable[rowIndex][columnIndex]->rowIndex = rowIndex;
			m_tileNodeTable[rowIndex][columnIndex]->columnIndex = columnIndex;

			m_tileNodeTable[rowIndex][columnIndex]->parent = nullptr;

			m_tileNodeTable[rowIndex][columnIndex]->isVisit = false;
		}
	}
}

void MapManager::drawTileTable()
{
	m_drawNode->clear();
	cocos2d::Color4F color = cocos2d::Color4F(164.0f / 255.0f, 72.0f / 255.0f, 192.0f / 255.0f, 1.0f);
	auto totalHeight = m_tileSize.height * m_mapSize.height;
	for (int rowIndex = 0; rowIndex < (int)m_mapSize.height; rowIndex++)
	{
		for (int columnIndex = 0; columnIndex < (int)m_mapSize.width; columnIndex++)
		{
			/*auto originPoint = cocos2d::Vec2(columnIndex * m_tileSize.width, totalHeight - rowIndex * m_tileSize.height);
			auto destPoint = cocos2d::Vec2((columnIndex + 1) * m_tileSize.width, totalHeight - (rowIndex + 1) * m_tileSize.height);
			if (m_tileNodeTable[rowIndex][columnIndex]->occupy == 1)
			{
				m_drawNode->drawSolidRect(originPoint, destPoint, color);
			}
			else
			{
				m_drawNode->drawRect(originPoint, destPoint, color);
			}*/
			auto curPoint = m_tileNodeTable[rowIndex][columnIndex]->position;
			cocos2d::Vec2 points[4];
			points[0].x = curPoint.x - m_tileSize.width / 2.0;
			points[0].y = curPoint.y;
			points[1].x = curPoint.x;
			points[1].y = curPoint.y + m_tileSize.height / 2.0;
			points[2].x = curPoint.x + m_tileSize.width / 2.0;
			points[2].y = curPoint.y;
			points[3].x = curPoint.x;
			points[3].y = curPoint.y - m_tileSize.height / 2.0;
			if (m_tileNodeTable[rowIndex][columnIndex]->occupy != OccupyType::Valid)
			{
				m_drawNode->drawSolidPoly(points, 4, color);
			}
			else
			{
				m_drawNode->drawPoly(points, 4, false, color);
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
	//WarFogLayer::getInstance()->setPosition(newPosition);
	//WarFogLayer::getInstance()->setPosition(newPosition, isDelta);
	//cocos2d::log("===>set map postion, x:%0.1f, y:%0.1f, delta y:%0.1f, scale:%0.1f", newPosition.x, newPosition.y, position.y, m_mapScale);
}


void MapManager::addChildToGameObjectLayer(cocos2d::Sprite* unit, int zOrder)
{
	m_gameObjectLayer->addChild(unit, zOrder);
	//cocos2d::log("child scale:%0.1f", unit->getScale());
}

cocos2d::Vec2 MapManager::getPosition()
{
	return m_tiledMap->getPosition();
}

const cocos2d::Size& MapManager::getTileSize()
{
	return m_tiledMap->getTileSize();
}

cocos2d::Size& MapManager::getMapSize()
{
	return m_mapSize;
}

cocos2d::Size& MapManager::getContentSize()
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
	//矩形地图行列计算
	/*auto tileSize = m_tiledMap->getTileSize();
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
	return cocos2d::Vec2(row, col);*/

	//菱形 45度地图行列计算
	//pos.x = ((col - row) / 2.0 + mapSize.width / 2.0) * tileSize.width;
	//pos.y = (mapSize.height - (col + row) / 2.0 + 0.5) * tileSize.height;
	
	//int row = m_mapSize.height - pos.y / m_tileSize.height - pos.x / m_tileSize.width + m_mapSize.width / 2.0 - 0.5;
	//int col = pos.x / m_tileSize.width + m_mapSize.height - m_mapSize.width / 2.0 - pos.y / m_tileSize.height - 0.5;
	float width = m_tileSize.width / 2.0;
	float height = m_tileSize.height / 2.0;
	int row = (m_tileMapOriginCoor.y * width - pos.y * width + m_tileMapOriginCoor.x * height - pos.x * height) / (2 * width * height) + 0.5;
	int col = (m_tileMapOriginCoor.y * width - pos.y * width + pos.x * height - m_tileMapOriginCoor.x * height) / (2 * width * height) + 0.5;
	
	if (row < 0 || row >= m_mapSize.height)
	{
		row = 0;
	}
	if (col < 0 || col >= m_mapSize.width)
	{
		col = 0;
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

void MapManager::setOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize, OccupyType type)
{
	auto rowCol = toTileRowCol(pos);
	if ((rowCol.x >= m_mapSize.height) || (rowCol.y >= m_mapSize.width))
	{
		return ;
	}

	int dtCol = contentSize.width / 2.0 / m_tileSize.width;
	int dtRow = contentSize.height / 2.0 / m_tileSize.height;
	cocos2d::Vec2 endRowCol = rowCol;
	endRowCol.x += dtRow;
	endRowCol.y += dtCol;
	if (endRowCol.x < 0)
	{
		return ;
	}

	rowCol.x -= dtRow;
	rowCol.y -= dtCol;

	int startRow = std::min(rowCol.x, endRowCol.x);
	startRow = std::max(0, startRow);

	int endRow = std::max(rowCol.x, endRowCol.x);
	endRow = std::min((float)endRow, m_mapSize.height);

	int startCol = std::min(rowCol.y, endRowCol.y);
	startCol = std::max(0, startCol);
	int endCol = std::max(rowCol.y, endRowCol.y);
	endCol = std::min((float)endCol, m_mapSize.width);

	for (int i = startRow; i <= endRow; ++i)
	{
		for (int j = startCol; j <= endCol; ++j)
		{
			cocos2d::log("map row:%d, col:%d, set row:%d, col:%d, set occupy", (int)rowCol.x, (int)rowCol.y, i, j);
			m_tileNodeTable[i][j]->occupy = type;
		}
	}
}

void MapManager::setOccupy(const int row, const int col, OccupyType type)
{
	if (row >= m_mapSize.height || col >= m_mapSize.width)
	{
		return;
	}
	if (row < 0 || col < 0)
	{
		return;
	}
	m_tileNodeTable[row][col]->occupy = type;
}

bool MapManager::checkOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize, OccupyType type)
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
			if (type == OccupyType::InValidBuilding)
			{
				if ((m_tileNodeTable[i][j]->occupy == OccupyType::InValid) ||
					(m_tileNodeTable[i][j]->occupy == OccupyType::Building))
				{
					return false;
				}
			}
			else if (m_tileNodeTable[i][j]->occupy == type)
			{
				cocos2d::log("row:%d, col:%d occupied", i, j);
				return false;
			}
		}
	}

	return true;
}

bool MapManager::checkOccupy(const int row, const int col, OccupyType type)
{
	if (row >= m_mapSize.height || col >= m_mapSize.width)
	{
		return false;
	}
	if (row < 0 || col < 0)
	{
		return false;
	}
	return m_tileNodeTable[row][col]->occupy != type;
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

cocos2d::Vec2 & MapManager::getObjectPosition(cocos2d::ValueMap& valueMap, std::string& name)
{
	float x = valueMap["x"].asFloat();
	float y = valueMap["y"].asFloat();
	int columnIndex = x / m_tileSize.height;
	int rowIndex = (m_mapSize.height * m_tileSize.height - y) / m_tileSize.height;
	return getTileNode(rowIndex, columnIndex)->position;
}

void MapManager::updateOccupy()
{

	for (int row = 0; row < m_mapSize.height; ++row)
	{
		for (int col = 0; col < m_mapSize.width; ++col)
		{
			if (m_tileNodeTable[row][col]->occupy == OccupyType::Soldier)
			{
				m_tileNodeTable[row][col]->occupy = OccupyType::Valid;
			}
		}
	}
	auto allSoldiers = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto& pairs : allSoldiers)
	{
		auto object = pairs.second;
		auto pos = object->getPosition();
		auto tilePos = toTileRowCol(pos);
		m_tileNodeTable[tilePos.x][tilePos.y]->occupy = OccupyType::Soldier;
	}
}
