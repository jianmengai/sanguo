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

	void setPlayerTeamPath(TeamNo teamNo, std::list<cocos2d::Vec2>& pathList);

	void selectPlayerTeam(TeamNo teamNo);

	void unSelectPlayer();

	int getPlayerTeamId(TeamNo teamNo);

	SOLDIER_MAP getPlayerSoldiers();

	void setPlayerTeam(TeamNo teamNo, std::vector<int>& teamMem);

	std::list<cocos2d::Vec2>& getPlayerTeamPath(TeamNo teamNo);

	void removeGameObject(GameObject* gameObject);

	bool playerHasBuilt();

	//返回true表示结束
	bool isGameEnd(bool isPlayerWin);
private:
	//设置基地位置
	void initBasePosition();
	void npcAttack();
	GameObject* npcGetAttackTarget();

	void updateVisible(float dt);
private:
	Army* m_player = nullptr;
	Army* m_npc = nullptr;

	float m_npcFindAttackTargetCdTime = 60;
	GameObject* m_npcAttackTarget = nullptr; //npc攻击目标

	bool m_playerStartBuilt = false;
	float m_lastUpdateVisibleCost = 0;
};