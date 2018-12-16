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
	auto barrack = barracks[0];
	if (barrack->getBuildingStatus() != BuildingStatus::Working)
	{
		return false;
	}
	auto soldierConf = GameConfig::getInstance()->getSoldierConf(m_forceType, type);
	auto pos = barracks[0]->getPosition();
	auto posToTileNode = MapManager::getInstance()->toTileRowCol(pos);
	auto barracksSize = barracks[0]->getContentSize();
	
	std::vector<SoldierPos> soldiersPos;
	auto findPos = getSoldierPos(pos, soldierConf->subCount, soldiersPos);
	if (!findPos)
	{
		cocos2d::log("no place to stand soldier");
		return false;
	}
	auto cost = soldierConf->technologyPoint;
	if (m_techPoint < (cost * soldierConf->subCount))
	{
		cocos2d::log("techPoint not enough");
		return false;
	}
	for (auto pos : soldiersPos)
	{
		Soldier* soldier = Soldier::create(m_forceType, type, pos.pos, pos.direction);
		if (nullptr == soldier)
		{
			continue;
		}
		m_techPoint -= cost;
		//cocos2d::log("===>create soldier, pos:[%0.1f, %0.1f]", newPos.x, newPos.y);
		m_soldiers[type].push_back(soldier);
		MapManager::getInstance()->addChildToGameObjectLayer(soldier);
		GameObjectManager::getInstance()->addGameObject(soldier);
		MapManager::getInstance()->setOccupy(pos.tileRow, pos.tileCol, OccupyType::Soldier);
	}
	
	return true;
}

bool Army::createBuilding(BuildingType type, const cocos2d::Vec2& position, bool isMapPos)
{
	if (!canBuild(type))
	{
		return false;
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

	auto buildingConf = GameConfig::getInstance()->getBuildingConf(m_forceType, type);
	auto cost = buildingConf->technologyPoint;
	if (m_techPoint < cost)
	{
		cocos2d::log("techPoint not enough");
		return false;
	}
	Building* building = Building::create(m_forceType, type, newPos);
	if (nullptr == building)
	{
		return false;
	}
	const auto& contentSize = building->getContentSize();
	if (!MapManager::getInstance()->checkOccupy(newPos, contentSize, OccupyType::InValidBuilding))
	{
		CC_SAFE_DELETE(building);
		cocos2d::log("can not building here...");
		return false;
	}
	m_techPoint -= cost;
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
	return true;
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

void Army::unSelect()
{
	m_selectedSodiers.clear();
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

void Army::removeSoldier(GameObject * gameObject)
{
	if (gameObject == nullptr)
	{
		return;
	}
	Soldier* soldier = dynamic_cast<Soldier*>(gameObject);
	auto soldierType = soldier->getSoldierType();
	auto& soldiers = m_soldiers[soldierType];
	for (auto it = soldiers.begin(); it != soldiers.end();)
	{
		if ((*it) == soldier)
		{
			it = soldiers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Army::removeBuilding(GameObject * gameObject)
{
	if (gameObject == nullptr)
	{
		return;
	}
	Building* building = dynamic_cast<Building*>(gameObject);
	auto buildingType = building->getBuildingType();
	auto& buildings = m_buildings[buildingType];
	for (auto it = buildings.begin(); it != buildings.end();)
	{
		if ((*it) == building)
		{
			it = buildings.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Army::npcAutoCreating()
{
	if (m_now - m_lastUpdateTime < 1)
	{
		return;
	}
	if (!isBuildingExist(BuildingType::MainTown))
	{
		createBuilding(BuildingType::MainTown, m_basePosition.basePosition, true);
	}
	if (!isBuildingExist(BuildingType::Barrack))
	{
		createBuilding(BuildingType::Barrack, m_basePosition.barrackPosition, true);
	}
	//检查防御塔
	for (auto pos : m_basePosition.archerTowerPositions)
	{
		auto archerTowers = m_buildings[BuildingType::DefenceTower];
		bool fund = false;
		for (auto tower : archerTowers)
		{
			if (tower == nullptr)
			{
				continue;
			}
			auto towerPos = tower->getPosition();
			if (towerPos == pos)
			{
				fund = true;
			}
		}
		if (!fund)
		{
			createBuilding(BuildingType::DefenceTower, pos, true);
		}
	}
	//这里直接根据一个编队需要的资源来创建，如果资源满足创建一个编队，则直接创建
	static auto archerConf = GameConfig::getInstance()->getSoldierConf(m_forceType, SoldierType::Archer);
	static auto cavalryConf = GameConfig::getInstance()->getSoldierConf(m_forceType, SoldierType::Cavalry);
	static auto infantryConf = GameConfig::getInstance()->getSoldierConf(m_forceType, SoldierType::Infantry);
	static int totalTechPoint = archerConf->technologyPoint * 2 + cavalryConf->technologyPoint * 2 + infantryConf->technologyPoint * 2;
	if (totalTechPoint <= m_techPoint)
	{
		createSoldier(SoldierType::Archer);
		createSoldier(SoldierType::Cavalry);
		createSoldier(SoldierType::Infantry);
		createSoldier(SoldierType::Archer);
		createSoldier(SoldierType::Cavalry);
		createSoldier(SoldierType::Infantry);
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
	//每秒增加一次
	if (((m_now - m_lastUpdateTime) >= 1))
	{
		if (!isMainTownWorking())
		{
			return;
		}
		if (m_forceType == ForceType::AI)
		{
			m_techPoint += 100;
		}
		else
		{
			m_techPoint += 100;
		}
		
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
	for (auto& pairs : m_teamPath)
	{
		auto teamNo = pairs.first;
		auto& path = pairs.second;
		if (path.empty())
		{
			continue;
		}
		auto pathPos = path.front();
		auto lastPos = m_teamLastPos[teamNo];
		//获取编队成员的状态以及坐标
		auto teamId = getTeamId(teamNo);
		auto& teamMem = TeamManager::getInstance()->getTeamMembers(teamId);
		bool shouldMove = true;
		float avgDistance = 0;
		for(auto mem : teamMem)
		{
			auto status = mem->getGameObjectStatus();
			auto objectPos = mem->getPosition();
			auto distance = GameUtils::computeDistanceBetween(pathPos, objectPos);
			avgDistance += distance;
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
			
			if (path.empty())
			{
				return;
			}
			pathPos = path.front();

			m_teamLastPos[teamNo] = pathPos;
			soldiersMoveTo(soldiers, pathPos);
		}

		avgDistance = avgDistance / teamMem.size();
		if ((avgDistance < 100) && (pathPos == lastPos))
		{
			path.pop_front();
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

void Army::getArroundNode(TileNode * node, int count, std::vector<TileNode*>& arrounds)
{
	if (node == nullptr)
	{
		return;
	}
	TileNode* lastNode = node;
	int y = node->columnIndex;
	int x = node->rowIndex;
	auto size = MapManager::getInstance()->getMapSize();
	int maxX = size.height;
	int maxY = size.width;
	for (int i = 1; i < 100; ++i)
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
					arrounds.push_back(lastNode);
					if (arrounds.size() == count)
					{
						return;
					}
				}
			}
		}
	}
}

bool Army::isMainTownWorking()
{
	auto it = m_buildings.find(BuildingType::MainTown);
	if (it == m_buildings.end())
	{
		return false;
	}
	auto buildings = it->second;
	bool fund = false;
	for (auto mainTown : buildings)
	{
		if (mainTown == nullptr)
		{
			continue;
		}
		if (mainTown->isWorking())
		{
			fund = true;
			break;
		}
	}
	return fund;
}

bool Army::getSoldierPos(cocos2d::Vec2& pos, int count, std::vector<SoldierPos>& soldiersPos)
{
	std::vector<cocos2d::Vec2> hasUsePos;
	auto posToTileNode = MapManager::getInstance()->toTileRowCol(pos);
	auto mapSize = MapManager::getInstance()->getMapSize();
	auto tileSize = MapManager::getInstance()->getTileSize();
	float centerX = mapSize.width * tileSize.width / 2;
	float centerY = mapSize.height * tileSize.height / 2;
	FaceDirection direction;
	cocos2d::Vec2 newPos = cocos2d::Vec2::ZERO;
	for (int i = 0; i < count; ++i)
	{
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
						newPos = MapManager::getInstance()->tileRowColToPos(row, col);
						auto it = std::find(hasUsePos.begin(), hasUsePos.end(), newPos);
						//已经找过
						if (it != hasUsePos.end())
						{
							continue;
						}
						if (MapManager::getInstance()->checkOccupy(row, col, OccupyType::Soldier))
						{
							hasUsePos.push_back(newPos);
							SoldierPos soldierPos;
							soldierPos.pos = newPos;
							soldierPos.tileRow = row;
							soldierPos.tileCol = col;
							soldierPos.direction = direction;
							soldiersPos.push_back(soldierPos);
							if (soldiersPos.size() == count)
							{
								return true;
							}
						}
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
						newPos = MapManager::getInstance()->tileRowColToPos(row, col);
						auto it = std::find(hasUsePos.begin(), hasUsePos.end(), newPos);
						//已经找过
						if (it != hasUsePos.end())
						{
							continue;
						}
						if (MapManager::getInstance()->checkOccupy(row, col, OccupyType::Soldier))
						{
							hasUsePos.push_back(newPos);
							SoldierPos soldierPos;
							soldierPos.pos = newPos;
							soldierPos.tileRow = row;
							soldierPos.tileCol = col;
							soldierPos.direction = direction;
							soldiersPos.push_back(soldierPos);
							if (soldiersPos.size() == count)
							{
								return true;
							}
						}
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
				for (int row = posToTileNode.x - 3; row <= static_cast<int>(mapSize.height); ++row)
				{
					for (int col = posToTileNode.y + 3; col <= posToTileNode.y + 7/*static_cast<int>(mapSize.width)*/; ++col)
					{
						cocos2d::log("try tile:[%d, %d]", row, col);
						newPos = MapManager::getInstance()->tileRowColToPos(row, col);
						auto it = std::find(hasUsePos.begin(), hasUsePos.end(), newPos);
						//已经找过
						if (it != hasUsePos.end())
						{
							continue;
						}
						if (MapManager::getInstance()->checkOccupy(row, col, OccupyType::Soldier))
						{
							hasUsePos.push_back(newPos);
							SoldierPos soldierPos;
							soldierPos.pos = newPos;
							soldierPos.tileRow = row;
							soldierPos.tileCol = col;
							soldierPos.direction = direction;
							soldiersPos.push_back(soldierPos);
							if (soldiersPos.size() == count)
							{
								return true;
							}
						}
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
						newPos = MapManager::getInstance()->tileRowColToPos(row, col);
						auto it = std::find(hasUsePos.begin(), hasUsePos.end(), newPos);
						//已经找过
						if (it != hasUsePos.end())
						{
							continue;
						}
						if (MapManager::getInstance()->checkOccupy(row, col, OccupyType::Soldier))
						{
							hasUsePos.push_back(newPos);
							SoldierPos soldierPos;
							soldierPos.pos = newPos;
							soldierPos.tileRow = row;
							soldierPos.tileCol = col;
							soldierPos.direction = direction;
							soldiersPos.push_back(soldierPos);
							if (soldiersPos.size() == count)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool Army::isBuildingExist(BuildingType type)
{
	auto it = m_buildings.find(type);
	if (it == m_buildings.end())
	{
		return false;
	}
	auto buildings = it->second;
	return buildings.empty() == false;
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
	m_now = time(nullptr);
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
	if (m_now - m_lastUpdateTime >= 1)
	{
		m_lastUpdateTime = m_now;
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
	int count = soldiers.size() - 1;
	if (count < 0)
	{
		return  false;
	}
	if (count == 0)
	{
		soldiers[0]->moveTo(mapPos);
	}
	else
	{
		std::vector<TileNode*> arrounds;
		getArroundNode(tileNode, count, arrounds);
		arrounds.push_back(tileNode);
		if (arrounds.size() != soldiers.size())
		{
			return false;
		}
		int index = 0;
		for (auto soldier : soldiers)
		{
			soldier->setTeamSpeed(teamSpeed);
			soldier->moveTo(arrounds[index]->position);
			++index;
		}
	}
	return true;
}

bool Army::soldiersMoveTo(const cocos2d::Vec2 & position)
{
	return soldiersMoveTo(m_selectedSodiers, position);
}
