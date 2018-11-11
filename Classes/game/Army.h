#pragma once
#include <unordered_map>
#include <vector>

#include "Building.h"
#include "Soldier.h"

typedef std::unordered_map<SoldierType, std::vector<Soldier*> > SOLDIER_MAP;
typedef std::unordered_map<BuildingType, std::vector<Building*> > BUILDING_MAP;

class Army
{
public:
	Army();
	~Army();

	void update(float dt);
	Soldier* createSoldier(SoldierType type);
	Building* createBuilding(BuildingType type, const cocos2d::Vec2& position, bool isMapPos = false);
	void soldiersMoveTo(const cocos2d::Vec2& position);

	void addSelected(GameObject* gameObject);
	void clearSelected();

	void attackTarget(GameObject* gameObject);

	void setForceType(ForceType forceType);

	void setBasePosition(BasePosition& position);

protected:
	//队伍中发现攻击目标，通知给队伍所有成员
	//void notifyTeamMemberTarget();

	void npcAutoCreating();
private:
	bool canBuild(BuildingType buildingType);
private:
	int m_gold = 0;
	SOLDIER_MAP m_soldiers;
	BUILDING_MAP m_buildings;
	std::vector<Soldier*> m_selectedSodiers;
	ForceType m_forceType;

	GameObject* m_attackTarget = nullptr; //指令攻击的目标
	BasePosition m_basePosition;
};
