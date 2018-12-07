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

	void touchProcess(const cocos2d::Vec2& position);
	void playerMoveTo(const cocos2d::Vec2& postiion);
	int getPlayerTechPoint();

	void setPath(TeamNo teamNo, std::list<cocos2d::Vec2>& pathList);

	int getTeamId(TeamNo teamNo);

	SOLDIER_MAP getPlayerSoldiers();

	void setPlayerTeam(TeamNo teamNo, std::vector<int>& teamMem);
private:
	//设置基地位置
	void initBasePosition();
	void npcAttack();
	GameObject* npcGetAttackTarget();

	void updateVisible();
private:
	Army* m_player = nullptr;
	Army* m_npc = nullptr;

	float m_npcFindAttackTargetCdTime = 60;
	GameObject* m_npcAttackTarget = nullptr; //npc攻击目标
};