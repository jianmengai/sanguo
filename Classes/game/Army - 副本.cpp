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
	TileNode* validNode = nullptr;
	bool find = false;
	if (m_forceType == ForceType::AI)
	{
		//朝向西
		direction = FaceDirection::FaceToWest;
		
		for (int col = posToTileNode.y + 4; col >=0; --col)
		{
			for (int row = posToTileNode.x - 3; row < posToTileNode.x + 3; ++row)
			{
				if ((col < 0) || (col >= mapSize.width))
				{
					continue;
				}
				if ((row < 0) || (row >= mapSize.height))
				{
					continue;
				}
				auto node = MapManager::getInstance()->getTileNode(row, col);
				if (node->occupy == OccupyType::Valid)
				{
					validNode = node;
					break;
				}
			}
			if (validNode != nullptr)
			{
				break;
			}
		}
	}
	else
	{
		//朝向东
		direction = FaceDirection::FaceToEast;
		for (int col = posToTileNode.y + 4; col <= mapSize.width; ++col)
		{
			for (int row = posToTileNode.x - 3; row < posToTileNode.x + 3; ++row)
			{
				if ((col < 0) || (col >= mapSize.width))
				{
					continue;
				}
				if ((row < 0) || (row >= mapSize.height))
				{
					continue;
				}
				auto node = MapManager::getInstance()->getTileNode(row, col);
				if (node->occupy == OccupyType::Valid)
				{
					validNode = node;
					break;
				}
			}
			if (validNode != nullptr)
			{
				break;
			}
		}
	}
	if (validNode == nullptr)
	{
		cocos2d::log("----no valid place----");
		return nullptr;
	}
	newPos = MapManager::getInstance()->toMapPos(cocos2d::Vec2(validNode->rowIndex, validNode->columnIndex));
	Soldier* soldier = Soldier::create(m_forceType, type, newPos, direction);
	if (nullptr == soldier)
	{
		return nullptr;
	}

	cocos2d::log("===>create soldier, pos:[%0.1f, %0.1f]", newPos.x, newPos.y);
	m_soldiers[type].push_back(soldier);
	MapManager::getInstance()->addChildToGameObjectLayer(soldier);
	GameObjectManager::getInstance()->addGameObject(soldier);
	MapManager::getInstance()->setOccupy(validNode->rowIndex, validNode->columnIndex, OccupyType::Soldier);
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
		auto pathPos = path.front();
		//获取编队成员的状态以及坐标
		auto teamId = getTeamId(teamNo);
		auto& teamMem = TeamManager::getInstance()->getTeamMembers(teamId);
		bool inPos = true;
		for(auto mem : teamMem)
		{
			auto status = mem->getGameObjectStatus();
			auto objectPos = mem->getPosition();
			auto distance = GameUtils::computeDistanceBetween(pathPos, objectPos);
			if (status != GameObjectStatus::Stand)
			{
				inPos = false;
				break;
			}
			if (distance > 600)
			{
				inPos = false;
			}
		}
		//if ()
		{

		}
	}
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
		if (soldiersMoveTo(gameObject->getPosition()))
		{
			m_attackTarget = gameObject;
		}
	}
}

bool Army::soldiersMoveTo(const cocos2d::Vec2& mapPos)
{
	if (m_selectedSodiers.empty())
	{
		selectTeam(TeamNo::One);
		if (m_selectedSodiers.empty())
		{
			return false;
		}
	}
	float teamSpeed = FLT_MAX;
	for (auto sodier : m_selectedSodiers)
	{
		if (teamSpeed > sodier->getMoveSpeed())
		{
			teamSpeed = sodier->getMoveSpeed();
		}
	}

	//auto mapPos = MapManager::getInstance()->toMapPos(position);
	auto endTileNode = MapManager::getInstance()->getTileNode(mapPos);
	int row = endTileNode->rowIndex;
	int col = endTileNode->columnIndex;
	auto moveToPos = mapPos;
	auto mapSize = MapManager::getInstance()->getMapSize();
	for (auto soldier : m_selectedSodiers)
	{
		soldier->setTeamSpeed(teamSpeed);
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

	return true;
}