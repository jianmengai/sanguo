#include "GameBattle.h"
#include "MapManager.h"
#include "GameObjectManager.h"

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
}

void GameBattle::playerMoveTo(const cocos2d::Vec2& postiion)
{
	m_player->soldiersMoveTo(postiion);
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