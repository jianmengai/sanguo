#pragma once

#include "GameDefine.h"
#include "Army.h"

class GameBattle
{
public:
	static GameBattle* getInstance();

	bool init();
	void clear();
	bool createBuilding(ForceType forceType, BuildingType buidingType, cocos2d::Vec2 position);
	bool createSoldier(ForceType forceType, SoldierType soldierType);

	void update(float dt);
private:
	Army* m_player = nullptr;
	Army* m_npc = nullptr;
};