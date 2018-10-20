#include "GameBattle.h"
#include "MapManager.h"

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
	m_npc = new (std::nothrow) Army;

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
		//获取兵营位置
		//遍历兵营周边未占用的位置
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

}