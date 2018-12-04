#include "GameBattle.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "GameConfig.h"
#include "WarfogLayer.h"

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
	return true;
}

bool GameBattle::createBuilding(ForceType forceType, BuildingType buidingType, cocos2d::Vec2 position)
{
	
	//cd检查
	//建造资源检查
	//不同建筑数量限制
	if (ForceType::Player == forceType)
	{
		return m_player->createBuilding(buidingType, position);
	}

	return true;
}

bool GameBattle::createSoldier(ForceType forceType, SoldierType soldierType)
{
	if (ForceType::Player == forceType)
	{
		return m_player->createSoldier(soldierType);
	}

	return true;
}

void GameBattle::update(float dt)
{
	m_player->update(dt);
	m_npc->update(dt);
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

	updateVisible();
}

void GameBattle::playerMoveTo(const cocos2d::Vec2& postiion)
{
	m_player->soldiersMoveTo(postiion);
}

int GameBattle::getPlayerTechPoint()
{
	return m_player->getTechPoint();
}

void GameBattle::setPath(int teamNo, std::list<cocos2d::Vec2>& pathList)
{
}

int GameBattle::getTeamId(TeamNo teamNo)
{
	return m_player->getTeamId(teamNo);
}

void GameBattle::touchProcess(const cocos2d::Vec2& position)
{
	auto mapPos = MapManager::getInstance()->toMapPos(position);
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
	BasePosition playerBase;
	BasePosition npcBase;
	auto& basePositions = MapManager::getInstance()->getBasePosition();
	do
	{
		auto index1 = rand() % basePositions.size();
		auto index2 = rand() % basePositions.size();
		if (index1 != index2)
		{
			playerBase = basePositions.at(index1);
			npcBase = basePositions.at(index2);
			break;
		}
	} while (true);
	//地图切到玩家视野
	MapManager::getInstance()->setPosition(playerBase.basePosition);
	//WarFogLayer::getInstance()->setFogPosition(playerBase.basePosition);
	m_player->setBasePosition(playerBase);
	m_npc->setBasePosition(npcBase);
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

void GameBattle::updateVisible()
{
	/*
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	auto& gameObjests = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto& objectPair1 : gameObjests)
	{
		auto object1 = objectPair1.second;
		if (object1->getForceType() == ForceType::AI)
		{
			continue;
		}
		auto position = object1->getPosition() * MapManager::getInstance()->getMapScale();
		cocos2d::Rect checkRect(position.x - winSize.width, position.y - winSize.height, winSize.width * 2, winSize.height * 2);
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
			}
			else
			{
				object2->setVisible(false);
			}
		}
	}
	*/
}
