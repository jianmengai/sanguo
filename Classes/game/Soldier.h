#pragma once
#include <list>
#include "GameObject.h"

struct TileNode;

class Soldier : public GameObject
{
public:
	static Soldier* create(ForceType forceType, SoldierType type, const cocos2d::Vec2& position, FaceDirection direction);

	void onAttackAnimationEnd();
	void onDieAnimationEnd();

	void clear();

	void update(float deltaTime);

	void attackTarget(GameObject* target);
	//寻路指令
	void moveTo(const cocos2d::Vec2& pos);
private:
	bool init(ForceType forceType, SoldierType type, const cocos2d::Vec2& position, FaceDirection direction);
	bool initAnimate(SoldierType type);
	bool initData(SoldierType type);
	cocos2d::RepeatForever* createAnimateWithPlist(const std::string& plistFile, float animateDelayPerUnit, GameObjectStatus status);

	FaceDirection getFaceDirection(const cocos2d::Vec2& moveToPos);
	float getMoveToDuration(const cocos2d::Vec2& moveToPos);

	bool isReadyToRemove();
	void onPrepareToRemove();
	void initHpBar();

	
	//npc自动寻找攻击对象，攻击
	void findAndFight(float deltaTime);
	void searchEnemy();
	bool isEnemyInAlertRange(GameObject* enemy);
	cocos2d::Vec2 getEnemyPosition(GameObject* enemy);
	bool isEnemyInAttackRange(GameObject* enemy);

	void toMove();
	void toStand();
	void toAttack();
	void toDie();

private:
	cocos2d::Size m_dieAnimationFrameSize;

	cocos2d::RepeatForever* m_dieAnimate = nullptr;
	FaceDirection m_faceDirection = FaceDirection::Invalid;

	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_moveAnimateMap;
	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_standAnimateMap;
	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_attackAnimateMap;

	std::list<TileNode*> m_pathList;
	int m_teamId = 0;
	GameObjectStatus m_soldierStatus;

	float m_moveSpeed = 50;
	int m_attackPs = 1;

	float m_alertTimeCd = 0;
};