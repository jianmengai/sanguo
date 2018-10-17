#pragma once

#include "cocos2d.h"
#include "GameDefine.h"
#include "GameObject.h"

class BulletManager
{
public:
	static BulletManager* getInstance();
	cocos2d::Sprite* createBullet(BulletType bulletType, int attackerId, int attackTargetId);
private:
	void onNormalDamageBulletMoveEnd(cocos2d::Sprite* bullet, BulletType bulletType, int attackTargetID, int damageAmount);
	void onAOEDamageBulletMoveEnd(cocos2d::Sprite* bullet, BulletType bulletType, GameObject* attacker, const cocos2d::Vec2& endPosition);
	void onCreateSpecialEffect(BulletType bulletType, const cocos2d::Vec2& inMapPosition);
};