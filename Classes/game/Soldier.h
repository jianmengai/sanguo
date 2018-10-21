#pragma once
#include <list>
#include "GameObject.h"

struct TileNode;

class Soldier : public GameObject
{
public:
	static Soldier* create(SoldierType type, const cocos2d::Vec2& position, FaceDirection direction);
	bool init(SoldierType type, const cocos2d::Vec2& position, FaceDirection direction);

	void onAttackAnimationEnd();
	void onDieAnimationEnd();

	void clear();

	void update(float deltaTime);

	//Ñ°Â·Ö¸Áî
	void moveTo(const cocos2d::Vec2& pos);
private:
	bool initAnimate(SoldierType type);
	bool initData(SoldierType type);
	cocos2d::RepeatForever* createAnimateWithPlist(const std::string& plistFile, float animateDelayPerUnit, GameObjectStatus status);

	FaceDirection getFaceDirection(const cocos2d::Vec2& moveToPos);
	float getMoveToDuration(const cocos2d::Vec2& moveToPos);

	void toMove();
	void toStand();
	void toAttack();

private:
	cocos2d::Size m_dieAnimationFrameSize;

	cocos2d::RepeatForever* m_dieAnimate = nullptr;
	FaceDirection m_faceDirection = FaceDirection::Invalid;

	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_moveAnimateMap;
	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_standAnimateMap;
	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_attackAnimateMap;

	std::list<TileNode*> m_pathList;
	
	GameObjectStatus m_soldierStatus;

	float m_moveSpeed = 50;
};