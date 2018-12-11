#include "Army.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "AutoFindPath.h"
#include "WarFogLayer.h"
#include "GameConfig.h"
#include "TeamManager.h"
#include "GameUtils.h"
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
	auto barrack = barracks[0];
	if (barrack->getBuildingStatus() != BuildingStatus::Working)
	{
		return nullptr;
	}
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
				for (int col = posToTileNode.y; col >= 0; --col)
				{
					cocos2d::log("try tile:[%d, %d]", row, col);
					if (MapManager::getInstance()->checkOccupy(row, col, OccupyType::Soldier))
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
					if (MapManager::getInstance()->checkOccupy(row, col, OccupyType::Soldier))
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
			for (int row = posToTileNode.x - 6; row <= static_cast<int>(mapSize.height); ++row)
			{
				for (int col = posToTileNode.y + 3; col <= posToTileNode.y + 7/*static_cast<int>(mapSize.width)*/; ++col)
				{
					cocos2d::log("try tile:[%d, %d]", row, col);
					if (MapManager::getInstance()->checkOccupy(row, col, OccupyType::Soldier))
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
					if (MapManager::getInstance()->checkOccupy(row, col, OccupyType::Soldier))
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
	Soldier* soldier = Soldier::create(m_forceType, type, newPos, direction);
	if (nullptr == soldier)
	{
		return nullptr;
	}

	cocos2d::log("===>create soldier, pos:[%0.1f, %0.1f]", newPos.x, newPos.y);
	m_soldiers[type].push_back(soldier);
	MapManager::getInstance()->addChildToGameObjectLayer(soldier);
	GameObjectManager::getInstance()->addGameObject(soldier);
	MapManager::getInstance()->setOccupy(tileRow, tileCol, OccupyType::Soldier);
	return soldier;
}

Building* Army::createBuilding(BuildingType type, const cocos2d::Vec2& position, bool isMapPos)
{
	if (!canBuild(type))
	{
		return nullptr;
	}
	cocos2d::Vec2 newPos;
	if (!isMapPos)
	{
		newPos = MapManager::getInstance()->toMapPos(position);
	}
	else
	{
		newPos = position;
	}

	Building* building = Building::create(m_forceType, type, newPos);
	if (nullptr == building)
	{
		return nullptr;
	}
	const auto& contentSize = building->getContentSize();
	if (!MapManager::getInstance()->checkOccupy(newPos, contentSize, OccupyType::InValidBuilding))
	{
		CC_SAFE_DELETE(building);
		cocos2d::log("can not building here...");
		return nullptr;
	}

	cocos2d::log("==> create building, pos:[x=%0.1f, y=%0.1f]\n", newPos.x, newPos.y);
	m_buildings[type].push_back(building);
	MapManager::getInstance()->addChildToGameObjectLayer(building);
	GameObjectManager::getInstance()->addGameObject(building);
	//MapManager::getInstance()->setOccupy(newPos, contentSize);
	
	//九宫格开视野
	auto posToTileNode = MapManager::getInstance()->toTileRowCol(newPos);
	auto tileSize = MapManager::getInstance()->getTileSize();
	for (int i = posToTileNode.x - 1; i <= posToTileNode.x + 1; ++i)
	{
		for (int j = posToTileNode.y - 1; j <= posToTileNode.y + 1; ++j)
		{
			if (i >= 0 && j >= 0 && i < tileSize.width && j < tileSize.height)
			{
				//WarFogLayer::getInstance()->inView(i, j);
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

int Army::getTechPoint()
{
	return m_techPoint;
}

void Army::selectTeam(TeamNo teamNo)
{
	//不管队列是否存在，清空当前选中的目标
	m_selectedSodiers.clear();

	auto it = m_teams.find(teamNo);
	if (it == m_teams.end())
	{
		return;
	}
	auto teamId = it->second;
	auto members = TeamManager::getInstance()->getTeamMembers(teamId);
	if (!members.empty())
	{
		
		for (auto member : members)
		{
			m_selectedSodiers.push_back(dynamic_cast<Soldier*>(member));
		}
	}
}

int Army::getTeamId(TeamNo teamNo)
{
	auto it = m_teams.find(teamNo);
	if (it == m_teams.end())
	{
		return 0;
	}
	return it->second;
}

void Army::setTeamPath(TeamNo teamNo, std::list<cocos2d::Vec2>& path)
{
	//auto teamId = getTeamId(teamNo);
	m_teamPath[teamNo] = path;
	/*
	auto& teamMem = TeamManager::getInstance()->getTeamMembers(teamId);
	for (auto gameObject : teamMem)
	{
		Soldier* soldier = dynamic_cast<Soldier*>(gameObject);
		soldier->setPath(path);
	}
	*/
}

std::list<cocos2d::Vec2>& Army::getTeamPath(TeamNo teamNo)
{
	return m_teamPath[teamNo];

}

SOLDIER_MAP Army::getAllSoldiers()
{
	for (auto& soldierIt : m_soldiers)
	{
		for (auto it = soldierIt.second.begin(); it != soldierIt.second.end();)
		{
			auto soldier = *it;
			if ((soldier == nullptr) || soldier->isReadyToRemove())
			{
				it = soldierIt.second.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	return m_soldiers;
}

void Army::npcAutoCreating()
{

	if (m_buildings.count(BuildingType::MainTown) == 0)
	{
		createBuilding(BuildingType::MainTown, m_basePosition.basePosition, true);
	}
	if (m_buildings.count(BuildingType::Barrack) == 0)
	{
		createBuilding(BuildingType::Barrack, m_basePosition.barrackPosition, true);
	}
	int teamOneCount = 0;
	auto it = m_teams.find(TeamNo::One);
	if ((it != m_teams.end()))
	{
		teamOneCount = TeamManager::getInstance()->getTeamMembers(it->second).size();
	}
	if (teamOneCount == 0)
	{
		auto archorSoldier = createSoldier(SoldierType::Archer);
		if (archorSoldier != nullptr)
		{
			addToTeam(TeamNo::One, archorSoldier);
		}
		/*
		auto cavalrySoldier = createSoldier(SoldierType::Cavalry);
		if (cavalrySoldier != nullptr)
		{
			addToTeam(TeamNo::One, cavalrySoldier);
		}
		auto infantrySoldier = createSoldier(SoldierType::Infantry);
		if (infantrySoldier != nullptr)
		{
			addToTeam(TeamNo::One, infantrySoldier);
		}
		*/
	}
}

bool Army::canBuild(BuildingType buildingType)
{
	bool canBuild = true;
	switch (buildingType)
	{
	case BuildingType::MainTown:
	{
		auto it = m_buildings.find(buildingType);
		if (it != m_buildings.end())
		{
			auto buildingConf = GameConfig::getInstance()->getBuildingConf(m_forceType, buildingType);
			if (buildingConf == nullptr)
			{
				canBuild = false;
			}
			else
			{
				auto& buildings = it->second;
				if (buildings.size() >= buildingConf->maxCount)
				{
					canBuild = false;
				}
			}
			
		}
	}
	break;
	case BuildingType::DefenceTower:
	case BuildingType::Barrack:
	{
		auto it = m_buildings.find(BuildingType::MainTown);
		if (it == m_buildings.end())
		{
			canBuild = false;
		}
		else
		{
			auto& buildings = it->second;
			if (buildings.empty())
			{
				canBuild = false;
			}
			else
			{
				auto mainTown = buildings.at(0);
				if (mainTown->getBuildingStatus() != BuildingStatus::Working)
				{
					canBuild = false;
				}
			}
		}
	}
	default:
		break;
	}
	return canBuild;
}

void Army::updateTechPoint(float dt)
{
	static time_t lastUpdateTime = time(nullptr);
	time_t now = time(nullptr);
	//每秒增加一次
	if ((now - lastUpdateTime) >= 1)
	{
		m_techPoint += 100;
		lastUpdateTime = now;
	}
}

void Army::updateSelectAndTeam()
{
	for (auto it = m_selectedSodiers.begin(); it != m_selectedSodiers.end();)
	{
		Soldier* soldier = *it;
		if (soldier != nullptr)
		{
			if (soldier->isReadyToRemove())
			{
				it = m_selectedSodiers.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			it = m_selectedSodiers.erase(it);
		}
	}

}

void Army::updateTeamPath()
{

	for (auto pairs : m_teamPath)
	{
		auto teamNo = pairs.first;
		auto& path = pairs.second;
		if (path.empty())
		{
			continue;
		}
		auto pathPos = path.front();
		auto& lastPos = m_teamLastPos[teamNo];
		if (pathPos == lastPos)
		{
			continue;
		}
		//获取编队成员的状态以及坐标
		auto teamId = getTeamId(teamNo);
		auto& teamMem = TeamManager::getInstance()->getTeamMembers(teamId);
		bool shouldMove = true;
		for(auto mem : teamMem)
		{
			auto status = mem->getGameObjectStatus();
			auto objectPos = mem->getPosition();
			auto distance = GameUtils::computeDistanceBetween(pathPos, objectPos);
			if (status != GameObjectStatus::Stand)
			{
				shouldMove = false;
				break;
			}
		}
		if (shouldMove)
		{
			std::vector<Soldier*> soldiers;
			for (auto mem : teamMem)
			{
				soldiers.push_back(dynamic_cast<Soldier*>(mem));
			}
			m_teamLastPos[teamNo] = pathPos;
			soldiersMoveTo(soldiers, pathPos);
		}
	}
}

TileNode * Army::getLastNode(TileNode * node, int index)
{
		if (node == nullptr)
		{
			return nullptr;
		}
		TileNode* lastNode = node;
		int y = node->columnIndex;
		int x = node->rowIndex;
		auto size = MapManager::getInstance()->getMapSize();
		int maxX = size.height;
		int maxY = size.width;
		for (int i = index; i < 100; ++i)
		{
			for (int curX = x - i; curX <= x + i; ++curX)
			{
				for (int curY = y - i; curY <= y + i; ++curY)
				{
					if ((curX == x) && (curY == y))
					{
						continue;
					}
					if ((curX < 0) || (curX >= maxX))
					{
						continue;
					}
					if ((curY < 0) || (curY >= maxY))
					{
						continue;
					}
					if (MapManager::getInstance()->checkOccupy(curX, curY, OccupyType::Soldier))
					{
						lastNode = MapManager::getInstance()->getTileNode(curX, curY);
						return lastNode;
					}
				}
			}
		}
		return lastNode;

}

void Army::resetTeam(TeamNo teamNo)
{
	auto teamId = getTeamId(teamNo);
	if (teamId == 0)
	{
		return;
	}
	TeamManager::getInstance()->clearTeam(teamId);

}

void Army::addToTeam(TeamNo teamNo, GameObject * object)
{
	auto it = m_teams.find(teamNo);
	auto teamId = 0;
	if (it != m_teams.end())
	{
		teamId = it->second;
	}
	if (teamId == 0)
	{
		teamId = TeamManager::getInstance()->getUniqTeamId();
		m_teams[teamNo] = teamId;
	}
	
	TeamManager::getInstance()->addTeam(teamId, object);
	Soldier* soldier = dynamic_cast<Soldier*>(object);
	soldier->setTeamNo(teamNo);
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
		for (auto it = soldierIt.second.begin(); it != soldierIt.second.end();)
		{
			Soldier* soldier = *it;
			if ((soldier == nullptr) || (soldier->isReadyToRemove()))
			{
				it = soldierIt.second.erase(it);
			}
			else
			{
				//是否战斗中
				++it;
			}
		}
	}

	updateTechPoint(dt);
	updateSelectAndTeam();
	updateTeamPath();
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
	//if (m_attackTarget != gameObject)
	{
		if (soldiersMoveTo(m_selectedSodiers, gameObject->getPosition()))
		{
			m_attackTarget = gameObject;
		}
	}
}

bool Army::soldiersMoveTo(const std::vector<Soldier*>& soldiers, const cocos2d::Vec2& mapPos)
{
	if (soldiers.empty())
	{
		return false;
	}
	float teamSpeed = FLT_MAX;
	for (auto sodier : soldiers)
	{
		if (teamSpeed > sodier->getMoveSpeed())
		{
			teamSpeed = sodier->getMoveSpeed();
		}
	}

	auto moveToPos = mapPos;
	auto mapSize = MapManager::getInstance()->getMapSize();

	auto tileNode = MapManager::getInstance()->getTileNode(mapPos);
	int index = 0;
	for (auto soldier : soldiers)
	{
		soldier->setTeamSpeed(teamSpeed);
		if (index == 0)
		{
			soldier->moveTo(mapPos);
		}
		else
		{
			auto newNode = getLastNode(tileNode, index);
			if (newNode != nullptr)
			{
				soldier->moveTo(newNode->position);
			}

		}
		++index;
	}

	return true;
}

bool Army::soldiersMoveTo(const cocos2d::Vec2 & position)
{
	return soldiersMoveTo(m_selectedSodiers, position);
}
