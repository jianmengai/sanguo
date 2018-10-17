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
	bool createSoldier(SoldierType type);
	bool createBuilding(BuildingType type, const cocos2d::Vec2& position);
private:
	int m_gold = 0;
	SOLDIER_MAP m_soldiers;
	BUILDING_MAP m_buildings;
};
