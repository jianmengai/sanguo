#pragma once
#include "GameObject.h"


class Soldier : public GameObject
{
public:
	static Soldier* create(SoldierType type, const cocos2d::Vec2& position, FaceDirection direction);
	bool init(SoldierType type, const cocos2d::Vec2& position, FaceDirection direction);

	void onAttackAnimationEnd();
	void onDieAnimationEnd();

	void clear();
private:
	bool initAnimate(SoldierType type);
	bool initData(SoldierType type);
	cocos2d::RepeatForever* createAnimateWithPlist(const std::string& plistFile, float animateDelayPerUnit, GameObjectStatus status);
private:
	cocos2d::Size m_dieAnimationFrameSize;

	cocos2d::RepeatForever* m_dieAnimate = nullptr;
	FaceDirection m_faceDirection = FaceDirection::Invalid;

	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_moveAnimateMap;
	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_standAnimateMap;
	std::unordered_map<FaceDirection, cocos2d::RepeatForever*> m_attackAnimateMap;
};