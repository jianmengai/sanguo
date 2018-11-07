#include "Army.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "AutoFindPath.h"
#include "WarFogLayer.h"

Army::Army()
{

}

Army::~Army()
{

}

Soldier* Army::createSoldier(SoldierType type)
{
	//获取兵营位置
	auto barrackIt = m_buildings.find(BuildingType::Barrack);
	if (barrackIt == m_buildings.end() || barrackIt->second.empty())
	{
		cocos2d::log("need building barrack first");
		return nullptr;
	}
	
	auto barracks = barrackIt->second;
	auto pos = barracks[0]->getPosition();
	auto posToTileNode = MapManager::getInstance()->toTileRowCol(pos);
	auto barracksSize = barracks[0]->getContentSize();
	//获取当前兵营相对中心的位置
	auto mapSize = MapManager::getInstance()->getMapSize();
	auto tileSize = MapManager::getInstance()->getTileSize();
	float centerX = mapSize.width * tileSize.width / 2;
	float centerY = mapSize.height * tileSize.height / 2;
	FaceDirection direction;
	cocos2d::Vec2 newPos = cocos2d::Vec2::ZERO;
	bool found = false;
	int tileRow = -1;
	int tileCol = -1;
	if (pos.x > centerX)
	{
		if (pos.y > centerY)
		{
			direction = FaceDirection::FaceToSouthWest;
			//寻找*中可用位置	
			/*
			
			     **##
			     **##
				 ****
				 ****
			*/
			for (int row = posToTileNode.x + 2; row <= static_cast<int>(mapSize.height); --row)
			{
				for (int col = posToTileNode.y; col >=0; --col)
				{
					cocos2d::log("try tile:[%d, %d]", row, col);
					if (MapManager::getInstance()->checkOccupy(row, col))
					{
						found = true;
						tileRow = row;
						tileCol = col;
						newPos = MapManager::getInstance()->tileRowColToPos(row, col);
						break;
					}
				}
				if (found)
				{
					break;
				}
			}
		}
		else
		{
			direction = FaceDirection::FaceToNorthWest;
			//在上方寻找
			/*

				*****
				*****
				***##
				***##
			
			*/
			for (int row = posToTileNode.x - 2; row >= 0; --row)
			{
				for (int col = posToTileNode.y; col >= 0; --col)
				{
					cocos2d::log("try tile:[%d, %d]", row, col);
					if (MapManager::getInstance()->checkOccupy(row, col))
					{
						found = true;
						tileRow = row;
						tileCol = col;
						newPos = MapManager::getInstance()->tileRowColToPos(row, col);
						break;
					}
				}
				if (found)
				{
					break;
				}
			}

		}
	}
	else
	{
		if (pos.y > centerY)
		{
			direction = FaceDirection::FaceToSouthEast;
			//在下方
			/*
				
				##**
				##**
				****
				****
			*/
			for (int row = posToTileNode.x + 2; row <= static_cast<int>(mapSize.height); ++row)
			{
				for (int col = posToTileNode.y; col <= static_cast<int>(mapSize.width); ++col)
				{
					cocos2d::log("try tile:[%d, %d]", row, col);
					if (MapManager::getInstance()->checkOccupy(row, col))
					{
						found = true;
						tileRow = row;
						tileCol = col;
						newPos = MapManager::getInstance()->tileRowColToPos(row, col);
						break;
					}
				}
				if (found)
				{
					break;
				}
			}
		}
		else
		{
			direction = FaceDirection::FaceToNorthEast;
			//在上方
			/*
				
				****
				****
				##**
				##**

			*/

			for (int row = posToTileNode.x - 2; row >= 0; --row)
			{
				for (int col = posToTileNode.y; col <= static_cast<int>(mapSize.width); ++col)
				{
					cocos2d::log("try tile:[%d, %d]", row, col);
					if (MapManager::getInstance()->checkOccupy(row, col))
					{
						found = true;
						tileRow = row;
						tileCol = col;
						newPos = MapManager::getInstance()->tileRowColToPos(row, col);
						break;
					}
				}
				if (found)
				{
					break;
				}
			}
		}
	}
	if (!found)
	{
		return nullptr;
		cocos2d::log("no place to create soldier!");
	}
	
	Soldier* soldier = Soldier::create(type, newPos, direction);
	if (nullptr == soldier)
	{
		return nullptr;
	}

	soldier->setForceType(m_forceType);
	cocos2d::log("===>create soldier, pos:[%0.1f, %0.1f]", newPos.x, newPos.y);
	m_soldiers[type].push_back(soldier);
	MapManager::getInstance()->addChildToGameObjectLayer(soldier);
	GameObjectManager::getInstance()->addGameObject(soldier);
	MapManager::getInstance()->setOccupy(newPos, soldier->getContentSize());
	WarFogLayer::getInstance()->inView(tileRow, tileCol);
	return soldier;
}

Building* Army::createBuilding(BuildingType type, const cocos2d::Vec2& position, bool isMapPos)
{
	cocos2d::Vec2 newPos;
	if (!isMapPos)
	{
		newPos = MapManager::getInstance()->toMapPos(position);
	}
	else
	{
		newPos = position;
	}

	Building* building = Building::create(type, newPos);
	if (nullptr == building)
	{
		return nullptr;
	}
	const auto& contentSize = building->getContentSize();
	if (!MapManager::getInstance()->checkOccupy(newPos, contentSize))
	{
		CC_SAFE_DELETE(building);
		cocos2d::log("can not building here...");
		return nullptr;
	}
	building->setForceType(m_forceType);
	cocos2d::log("==> create building, pos:[x=%0.1f, y=%0.1f]\n", newPos.x, newPos.y);
	m_buildings[type].push_back(building);
	MapManager::getInstance()->addChildToGameObjectLayer(building);
	GameObjectManager::getInstance()->addGameObject(building);
	MapManager::getInstance()->setOccupy(newPos, contentSize);
	
	//九宫格开视野
	auto posToTileNode = MapManager::getInstance()->toTileRowCol(newPos);
	auto tileSize = MapManager::getInstance()->getTileSize();
	for (int i = posToTileNode.x - 1; i <= posToTileNode.x + 1; ++i)
	{
		for (int j = posToTileNode.y - 1; j <= posToTileNode.y + 1; ++j)
		{
			if (i >= 0 && j >= 0 && i < tileSize.width && j < tileSize.height)
			{
				WarFogLayer::getInstance()->inView(i, j);
			}
		}
	}
	return building;
}

void Army::setForceType(ForceType forceType)
{
	m_forceType = forceType;
}

void Army::setBasePosition(BasePosition & position)
{
	m_basePosition = position;
}

void Army::npcAutoCreating()
{
	//
	if (m_buildings.count(BuildingType::MainTown) == 0)
	{
		createBuilding(BuildingType::MainTown, m_basePosition.basePosition, true);
	}
	if (m_buildings.count(BuildingType::Barrack) == 0)
	{
		createBuilding(BuildingType::Barrack, m_basePosition.barrackPosition, true);
	}
	if (m_soldiers.size() < 3)
	{
		auto soldier = createSoldier(SoldierType::Archer);
		if (soldier != nullptr)
		{
			m_selectedSodiers.push_back(soldier);
		}
		soldier = createSoldier(SoldierType::Cavalry);
		if (soldier != nullptr)
		{
			m_selectedSodiers.push_back(soldier);
		}
		soldier = createSoldier(SoldierType::Infantry);
		if (soldier != nullptr)
		{
			m_selectedSodiers.push_back(soldier);
		}
	}
}

void Army::update(float dt)
{
	if (m_forceType == ForceType::AI)
	{
		npcAutoCreating();
	}
	//士兵非战斗状态非满HP自动回复生命值
	for (auto& soldierIt : m_soldiers)
	{

	}
	//建筑cd
	for (auto buildingIt : m_buildings)
	{

	}
}

void Army::addSelected(GameObject* gameObject)
{
	if (gameObject->getGameObjectType() == GameObjectType::Soldier)
	{
		Soldier* soldier = dynamic_cast<Soldier*>(gameObject);
		soldier->isSelected(true);
		m_selectedSodiers.push_back(dynamic_cast<Soldier*>(soldier));
		cocos2d::log("select soldier:%d", soldier->getId());
	}
}

void Army::clearSelected()
{
	for (auto soldier : m_selectedSodiers)
	{
		soldier->isSelected(false);
	}
	m_selectedSodiers.clear();
}

void Army::attackTarget(GameObject* gameObject)
{
	//if (gameObject->getForceType() != ForceType::AI)
	//{
	//	return;
	//}
	if (m_attackTarget != gameObject)
	{
		m_attackTarget = gameObject;
		soldiersMoveTo(gameObject->getPosition());
	}
}

void Army::soldiersMoveTo(const cocos2d::Vec2& mapPos)
{
	if (m_selectedSodiers.empty())
	{
		return;
	}
	//auto mapPos = MapManager::getInstance()->toMapPos(position);
	auto endTileNode = MapManager::getInstance()->getTileNode(mapPos);
	int row = endTileNode->rowIndex;
	int col = endTileNode->columnIndex;
	auto moveToPos = mapPos;
	auto mapSize = MapManager::getInstance()->getMapSize();
	for (auto& soldier : m_selectedSodiers)
	{
		soldier->attackTarget(m_attackTarget);
		soldier->moveTo(moveToPos);
		if (row - 1 >= 0)
		{
			TileNode* tileNode = MapManager::getInstance()->getTileNode(row -1, col);
			moveToPos = tileNode->position;
			row = row - 1;
			continue;
		}
		if (col - 1 >= 0)
		{
			TileNode* tileNode = MapManager::getInstance()->getTileNode(row, col -1);
			col = col - 1;
			moveToPos = tileNode->position;
			continue;
		}
	}
}