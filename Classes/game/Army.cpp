#include "Army.h"
#include "MapManager.h"
#include "GameObjectManager.h"

Army::Army()
{

}

Army::~Army()
{

}

bool Army::createSoldier(SoldierType type)
{
	//获取兵营位置
	auto barrackIt = m_buildings.find(BuildingType::Barrack);
	if (barrackIt == m_buildings.end() || barrackIt->second.empty())
	{
		cocos2d::log("need building barrack first");
		return false;
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
		return false;
		cocos2d::log("no place to create soldier!");
	}
	Soldier* soldier = Soldier::create(type, newPos, direction);
	if (nullptr == soldier)
	{
		return false;
	}
	cocos2d::log("===>create soldier, pos:[%0.1f, %0.1f]", newPos.x, newPos.y);
	m_soldiers[type].push_back(soldier);
	MapManager::getInstance()->addChildToGameObjectLayer(soldier);
	GameObjectManager::getInstance()->addGameObject(soldier);
	MapManager::getInstance()->setOccupy(newPos, soldier->getContentSize());
	return true;
}

bool Army::createBuilding(BuildingType type, const cocos2d::Vec2& position)
{
	auto newPos = MapManager::getInstance()->toMapPos(position);
	Building* building = Building::create(type, newPos);
	if (nullptr == building)
	{
		return false;
	}
	const auto& contentSize = building->getContentSize();
	if (!MapManager::getInstance()->checkOccupy(newPos, contentSize))
	{
		CC_SAFE_DELETE(building);
		cocos2d::log("can not building here...");
		return false;
	}
	cocos2d::log("==> create building, pos:[x=%0.1f, y=%0.1f]\n", newPos.x, newPos.y);
	m_buildings[type].push_back(building);
	MapManager::getInstance()->addChildToGameObjectLayer(building);
	GameObjectManager::getInstance()->addGameObject(building);
	MapManager::getInstance()->setOccupy(newPos, contentSize);
	return true;
}

void Army::update(float dt)
{
	//士兵非战斗状态非满HP自动回复生命值
	for (auto& soldierIt : m_soldiers)
	{

	}
	//建筑cd
	for (auto buildingIt : m_buildings)
	{

	}
}