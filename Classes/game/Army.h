#pragma once
#include <map>
#include <vector>

#include "Building.h"
#include "Soldier.h"

typedef std::map<SoldierType, std::vector<Soldier*> > SOLDIER_MAP;
typedef std::map<BuildingType, std::vector<Building*> > BUILDING_MAP;

typedef struct struct_soldier_pos_st
{
	cocos2d::Vec2 pos;
	int tileRow;
	int tileCol;
	FaceDirection direction;
}SoldierPos;

class Army
{
public:
	Army();
	~Army();

	void update(float dt);
	bool createSoldier(SoldierType type);
	bool createBuilding(BuildingType type, const cocos2d::Vec2& position, bool isMapPos = false);
	bool soldiersMoveTo(const std::vector<Soldier*>& soldiers, const cocos2d::Vec2& position);
	bool soldiersMoveTo(const cocos2d::Vec2& position);
	void addSelected(GameObject* gameObject);
	void clearSelected();

	void attackTarget(GameObject* gameObject);

	void setForceType(ForceType forceType);

	void setBasePosition(BasePosition& position);

	int getTechPoint();

	void selectTeam(TeamNo teamNo);

	void unSelect();

	int getTeamId(TeamNo teamNo);

	//��ն���
	void resetTeam(TeamNo teamNo);
	//��ӳ�Ա��������
	void addToTeam(TeamNo teamNo, GameObject* object);

	//���ö����н�·��
	void setTeamPath(TeamNo teamNo, std::list<cocos2d::Vec2>& path);

	std::list<cocos2d::Vec2>& getTeamPath(TeamNo teamNo);
	
	SOLDIER_MAP getAllSoldiers();

	void removeSoldier(GameObject* gameObject);
	void removeBuilding(GameObject* gameObject);
	bool isBuildingExist(BuildingType type);
protected:
	//�����з��ֹ���Ŀ�֪꣬ͨ���������г�Ա
	//void notifyTeamMemberTarget();

	void npcAutoCreating();
private:
	bool canBuild(BuildingType buildingType);
	void updateTechPoint(float dt);

	void updateSelectAndTeam();

	void updateTeamPath();

	TileNode* getLastNode(TileNode* node, int index);
	void getArroundNode(TileNode* node, int count, std::vector<TileNode*>& arrounds);
	bool isMainTownWorking();
	bool getSoldierPos(cocos2d::Vec2& pos, int count, std::vector<SoldierPos>& soldiersPos);
private:
	int m_techPoint = 3000;
	SOLDIER_MAP m_soldiers;
	BUILDING_MAP m_buildings;
	std::vector<Soldier*> m_selectedSodiers;
	ForceType m_forceType;

	GameObject* m_attackTarget = nullptr; //ָ�����Ŀ��
	BasePosition m_basePosition;

	std::map<TeamNo, int> m_teams;  //key:team number, value:team id

	std::map<TeamNo, std::list<cocos2d::Vec2> > m_teamPath;
	std::map<TeamNo, cocos2d::Vec2> m_teamLastPos;

	time_t m_lastUpdateTime = 0;
	time_t m_now = 0;
};
