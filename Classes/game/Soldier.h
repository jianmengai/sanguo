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

	float getMoveSpeed() { return m_moveSpeed; }
	void setTeamSpeed(float speed);
	void resumeNormalSpeed();

	bool isReadyToRemove();

	void setPath(std::list<cocos2d::Vec2>& path);
private:
	bool init(ForceType forceType, SoldierType type, const cocos2d::Vec2& position, FaceDirection direction);
	bool initAnimate(SoldierType type);
	bool initData(SoldierType type);
	cocos2d::RepeatForever* createAnimateWithPlist(const std::string& plistFile, float animateDelayPerUnit, GameObjectStatus status);

	FaceDirection getFaceDirection(const cocos2d::Vec2& moveToPos);
	float getMoveToDuration(const cocos2d::Vec2& startPos, const cocos2d::Vec2& moveToPos);

	bool isEnemyDiappear(GameObject* enemy);

	
	void onPrepareToRemove();
	void initHpBar();

	//寻路时，如果终点格子被占用，则选取格子周边的未被占用的格子
	TileNode* adjustEndNode(TileNode* endNode);
	
	//npc自动寻找攻击对象，攻击
	void findAndFight(float deltaTime);
	GameObject* searchEnemy();
	GameObject* searchNearbyEnemy();
	bool isEnemyInAlertRange(GameObject* enemy, float& distance);
	float getEnemyDistance(GameObject* enemy);
	cocos2d::Vec2 getEnemyPosition(GameObject* enemy);
	cocos2d::Vec2 getEnemyNearByPosition(cocos2d::Vec2& enemyPosition);
	bool isEnemyInAttackRange(GameObject* enemy);

	void toMove();
	void toStand();
	void toAttack();
	void toDie();

	void clearMoveToRowCol();

	void updatePreparePath();
	bool canMoveToPos(cocos2d::Vec2& pos);
	void movePath();

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
	float m_curSpeed = 50;
	int m_attackPs = 1;

	float m_alertTimeCd = 0;

	cocos2d::Vec2 m_moveToPosRowCol;
	bool m_moveToEnemy = false;
	bool m_attacking = false;

	//预设路线
	std::list<cocos2d::Vec2> m_preparePathList;
};