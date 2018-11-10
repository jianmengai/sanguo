#include "SpecialEffectManager.h"
#include "GameUtils.h"
#include "GameConfig.h"

SpecialEffectManager* SpecialEffectManager::getInstance()
{
	static SpecialEffectManager specialEffectManagerInstance;
	return &specialEffectManagerInstance;
}


cocos2d::Sprite* SpecialEffectManager::createSpecialEffect(const std::string& effectName, const cocos2d::Vec2& inMapPosition, bool isRepeat)
{
	if (effectName == "null")
	{
		return nullptr;
	}
	auto specialEffectConf =  GameConfig::getInstance()->getSpecialEffectConf(effectName);

	auto effectSprite = cocos2d::Sprite::create();
	effectSprite->setPosition(inMapPosition);

	auto animation = GameUtils::createAnimationWithPList(specialEffectConf->animationPListName);
	animation->setRestoreOriginalFrame(isRepeat);
	animation->setDelayPerUnit(specialEffectConf->perUnitIntervalBySecond);
	auto animate = cocos2d::Animate::create(animation);

	auto removeSpecialEffect = cocos2d::CallFunc::create(CC_CALLBACK_0(SpecialEffectManager::onRemoveSpecialEffect, this, effectSprite));
	auto sequenceAction = cocos2d::Sequence::create(animate, removeSpecialEffect, nullptr);
	effectSprite->runAction(sequenceAction);

	return effectSprite;
}

void SpecialEffectManager::onRemoveSpecialEffect(cocos2d::Sprite* specialEffect)
{
	specialEffect->retain();
	specialEffect->removeFromParent();
	specialEffect->autorelease();
}
