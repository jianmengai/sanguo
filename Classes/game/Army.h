#pragma once
#include <map>
#include <vector>

#include "Building.h"
#include "Soldier.h"

typedef std::map<SoldierType, std::vector<Soldier*> > SOLDIER_MAP;
typedef std::map<BuildingType, std::vector<Building*> > BUILDING_MAP;

class Army
{
public:
	Army();
	~Army();

	void update(float dt);
	Soldier* createSoldier(SoldierType type);
	Building* createBuilding(BuildingType type, const cocos2d::Vec2& position, bool isMapPos = false);
	bool soldiersMoveTo(const cocos2d::Vec2& position);

	void addSelected(GameObject* gameObject);
	void clearSelected();

	void attackTarget(GameObject* gameObject);

	void setForceType(ForceType forceType);

	void setBasePosition(BasePosition& position);

	int getTechPoint();

	void selectTeam(TeamNo teamNo);

protected:
	//队伍中发现攻击目标，通知给队伍所有成员
	//void notifyTeamMemberTarget();

	void npcAutoCreating();
private:
	bool canBuild(BuildingType buildingType);
	void updateTechPoint(float dt);

	void updateSelectAndTeam();

	void addToTeam(TeamNo teamNo, GameObject* object);
private:
	int m_techPoint = 1000;
	SOLDIER_MAP m_soldiers;
	BUILDING_MAP m_buildings;
	std::vector<Soldier*> m_selectedSodiers;
	ForceType m_forceType;

	GameObject* m_attackTarget = nullptr; //指令攻击的目标
	BasePosition m_basePosition;

	std::map<TeamNo, int> m_teams;  //key:team number, value:team id
};
