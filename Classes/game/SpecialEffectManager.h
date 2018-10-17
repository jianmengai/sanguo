#pragma once

#include "cocos2d.h"

class SpecialEffectManager
{
public:
	static SpecialEffectManager* getInstance();
	cocos2d::Sprite* createSpecialEffect(const std::string& effectName, const cocos2d::Vec2& inMapPosition, bool isRepeat);
private:
	void onRemoveSpecialEffect(cocos2d::Sprite* specialEffect);

	//SpecialEffectManager() {};
	//SpecialEffectManager(const SpecialEffectManager&);
	//SpecialEffectManager& operator = (const SpecialEffectManager&);
};