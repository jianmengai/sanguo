#include "GameBattle.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "GameConfig.h"
#include "WarfogLayer.h"
#include "TeamManager.h"

GameBattle* GameBattle::getInstance()
{
	static GameBattle gameBattleInstance;
	static bool hasInit = false;
	if (!hasInit)
	{
		gameBattleInstance.init();
		hasInit = true;
	}
	return &gameBattleInstance;
}

void GameBattle::clear()
{
}

bool GameBattle::init()
{
	m_player = new (std::nothrow) Army;
	m_player->setForceType(ForceType::Player);
	m_npc = new (std::nothrow) Army;
	m_npc->setForceType(ForceType::AI);
	initBasePosition();
	return initPlayerScout();
}

bool GameBattle::createBuilding(ForceType forceType, BuildingType buidingType, cocos2d::Vec2 position)
{
	
	//cd检查
	//建造资源检查
	//不同建筑数量限制
	if (ForceType::Player == forceType)
	{
		bool buildResult = m_player->createBuilding(buidingType, position);
		if ((!m_playerStartBuilt) && buildResult)
		{
			m_playerStartBuilt = true;
		}
		return buildResult;
	}

	return true;
}

bool GameBattle::createSoldier(ForceType forceType, SoldierType soldierType)
{
	if (ForceType::Player == forceType)
	{
		bool buildResult = m_player->createSoldier(soldierType);
		if ((!m_playerStartBuilt) && buildResult)
		{
			m_playerStartBuilt = true;
		}
		return buildResult;
	}

	return true;
}

void GameBattle::update(float dt)
{
	auto time1 = clock();
	updateVisible(dt);
	auto time2 = clock();
	cocos2d::log("========> update visible %u", time2 - time1);
	time1 = clock();
	m_player->update(dt);
	time2 = clock();
	cocos2d::log("========> update player %u", time2 - time1);
	time1 = clock();
	m_npc->update(dt);
	time2 = clock();
	cocos2d::log("========> update npc %u", time2 - time1);
	time1 = clock();
	if (m_npcFindAttackTargetCdTime >= GameConfig::getInstance()->getCooldownConf()->npcFindTargetCdTime)
	{
		npcAttack();
		m_npcFindAttackTargetCdTime = 0;
	}
	else
	{
		m_npcFindAttackTargetCdTime += dt;
	}
	GameObjectManager::getInstance()->removeAllReadyToRemoveGameObjects();

	TeamManager::getInstance()->update(dt);
	time2 = clock();
	cocos2d::log("========> update others %u", time2 - time1);
}

void GameBattle::playerMoveTo(const cocos2d::Vec2& postiion)
{
	m_player->soldiersMoveTo(postiion);
}

int GameBattle::getPlayerTechPoint()
{
	return m_player->getTechPoint();
}

void GameBattle::setPlayerTeamPath(TeamNo teamNo, std::list<cocos2d::Vec2>& pathList)
{
	m_player->setTeamPath(teamNo, pathList);
}

void GameBattle::selectPlayerTeam(TeamNo teamNo)
{
	m_player->selectTeam(teamNo);
}

void GameBattle::unSelectPlayer()
{
	m_player->unSelect();
}

int GameBattle::getPlayerTeamId(TeamNo teamNo)
{
	return m_player->getTeamId(teamNo);
}

SOLDIER_MAP GameBattle::getPlayerSoldiers()
{
	return m_player->getAllSoldiers();
}

void GameBattle::setPlayerTeam(TeamNo teamNo, std::vector<int>& teamMem)
{
	m_player->resetTeam(teamNo);
	for (auto gameObjectId : teamMem)
	{
		auto gameObject = GameObjectManager::getInstance()->getGameObjectById(gameObjectId);
		if ((gameObject != nullptr) && (!gameObject->isReadyToRemove()))
		{
			m_player->addToTeam(teamNo, gameObject);
		}
	}
}

std::list<cocos2d::Vec2>& GameBattle::getPlayerTeamPath(TeamNo teamNo)
{
	return m_player->getTeamPath(teamNo);
}

void GameBattle::removeGameObject(GameObject * gameObject)
{
	if (gameObject == nullptr)
	{
		return;
	}
	auto forceType = gameObject->getForceType();
	auto objectType = gameObject->getGameObjectType();
	Army* pArmy = nullptr;
	if (forceType == ForceType::AI)
	{
		pArmy = m_npc;
	}
	else if (forceType == ForceType::Player)
	{
		pArmy = m_player;
	}

	if (pArmy == nullptr)
	{
		return;
	}

	if (objectType == GameObjectType::Building)
	{
		pArmy->removeBuilding(gameObject);
	}
	else if (objectType == GameObjectType::Soldier)
	{
		pArmy->removeSoldier(gameObject);
	}
}

bool GameBattle::playerHasBuilt()
{
	return m_player->isBuildingExist(BuildingType::MainTown);
}

bool GameBattle::isGameEnd(bool& isPlayerWin)
{
	if (!m_playerStartBuilt)
	{
		return false;
	}
	bool isGameEnd = false;
	bool playerAllDie = true;
	bool npcAllDie = true;
	auto& allObjects = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto& objectPair : allObjects)
	{
		auto object = objectPair.second;
		if (object == nullptr)
		{
			continue;
		}
		if ((object->getForceType() == ForceType::AI) && (!object->isReadyToRemove()))
		{
			npcAllDie = false;
		}
		else if ((object->getForceType() == ForceType::Player) && (!object->isReadyToRemove()))
		{
			playerAllDie = false;
		}
	}

	if (playerAllDie)
	{
		isPlayerWin = false;
		isGameEnd = true;
	}
	else if (npcAllDie)
	{
		isPlayerWin = true;
		isGameEnd = true;
	}

	return isGameEnd;
}

void GameBattle::touchProcess(const cocos2d::Vec2& position)
{
	auto mapPos = MapManager::getInstance()->toMapPos(position);
	auto tileRowCol = MapManager::getInstance()->toTileRowCol(mapPos);
	cocos2d::log("!!!!!!!!!!%0.1f, %0.1f!!!!!!!!!!!!!!!!!", tileRowCol.x, tileRowCol.y);
	auto& gameObjects = GameObjectManager::getInstance()->getGameObjectMap();
	GameObject* selectObject = nullptr;
	ForceType foceType = ForceType::Invalid;
	for (auto& it : gameObjects)
	{
		auto gameObject = it.second;
		if (gameObject->getBoundingBox().containsPoint(mapPos))
		{
			selectObject = gameObject;
			foceType = selectObject->getForceType();
		}
	}
	//点击空地，移动选中单位
	if (selectObject == nullptr)
	{
		m_player->soldiersMoveTo(mapPos);
	}
	//点击己方单位，选中该单位
	else if (foceType == ForceType::Player)
	{
		m_player->clearSelected();
		m_player->addSelected(selectObject);
	}
	//点击敌方单位，攻击该单位
	else if (foceType == ForceType::AI)
	{
		m_player->attackTarget(selectObject);
	}
	else
	{
		cocos2d::log("invalid touch");
	}
}

void GameBattle::initBasePosition()
{
	//BasePosition playerBase;
	//BasePosition npcBase;
	auto& basePosition = MapManager::getInstance()->getBasePosition();
	auto& playerPos = MapManager::getInstance()->getPlayerInitPosition();
	auto pos = playerPos - cocos2d::Vec2(650, 960);
	//地图切到玩家视野
	MapManager::getInstance()->setPosition(pos);
	//WarFogLayer::getInstance()->setFogPosition(playerBase.basePosition);
	//m_player->setBasePosition(playerBase);
	m_npc->setBasePosition(basePosition);
}

bool GameBattle::initPlayerScout()
{
	m_playerStartBuilt = true;
	auto scoutPos = MapManager::getInstance()->getPlayerScoutPosition();
	return m_player->createScout(scoutPos);
}

void GameBattle::npcAttack()
{
	/*
	if ((m_npcAttackTarget == nullptr))
	{
		cocos2d::log("find target");
		m_npcAttackTarget = npcGetAttackTarget();
	}
	if (m_npcAttackTarget != nullptr)
	{
		m_npc->attackTarget(m_npcAttackTarget);
	}
	if ((m_npcAttackTarget != nullptr) && m_npcAttackTarget->isReadyToRemove())
	{
		cocos2d::log("current target is null");
		m_npcAttackTarget = nullptr;
	}
	*/
}
	

GameObject* GameBattle::npcGetAttackTarget()
{
	std::vector<GameObject*> targetBuildings;
	GameObject* target = nullptr;
	//这里直接获取玩家建筑，作为攻击目标
	auto& gameObjects = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto gameObjectIt : gameObjects)
	{
		auto& gameObject = gameObjectIt.second;
		if ((gameObject->getGameObjectType() == GameObjectType::Building) && (gameObject->getForceType() == ForceType::Player))
		{
			//优先攻击主城
			if (dynamic_cast<Building*>(gameObject)->getBuildingType() == BuildingType::MainTown)
			{
				target = gameObject;
			}
			else
			{
				targetBuildings.push_back(gameObject);
			}
		}
	}
	if ((target == nullptr) && (!targetBuildings.empty()))
	{
		auto size = targetBuildings.size();
		auto index = rand() % size;
		target = targetBuildings.at(index);

		cocos2d::log("find attack target...");
	}

	return target;
}

void GameBattle::updateVisible(float dt)
{
	m_lastUpdateVisibleCost += dt;
	if (m_lastUpdateVisibleCost < GameConfig::getInstance()->getCooldownConf()->updateVisible)
	{
		return;
	}
	m_lastUpdateVisibleCost = 0;
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	auto& gameObjests = GameObjectManager::getInstance()->getGameObjectMap();
	std::map<GameObject*, bool> hasSet;
	for (auto& objectPair1 : gameObjests)
	{
		auto object1 = objectPair1.second;
		if ((object1->getForceType() == ForceType::AI))
		{
			//object1->setVisible(false);
			continue;
		}
		auto position = object1->getPosition() * MapManager::getInstance()->getMapScale();
		cocos2d::Rect checkRect(position.x - winSize.width / 2, position.y - winSize.height / 2, winSize.width, winSize.height);
		for (auto& objectPair2 : gameObjests)
		{
			auto object2 = objectPair2.second;
			if (object2->getForceType() == ForceType::Player)
			{
				continue;
			}
			auto enemyPos = object2->getPosition() * MapManager::getInstance()->getMapScale();
			if (checkRect.containsPoint(enemyPos))
			{
				object2->setVisible(true);
				hasSet[object2] = true;
			}
			else
			{
				//如果已经visible已经设置为true，则本轮不能设置为false
				if (hasSet.count(object2) && hasSet[object2])
				{
					continue;
				}
				object2->setVisible(false);
			}
		}
	}
	
}
