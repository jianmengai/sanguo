#include "AnimButton.h"

AnimBaseButton::AnimBaseButton(void)
{
	m_isForever = false;
	m_pAnimNode = nullptr;
}

AnimBaseButton::~AnimBaseButton(void)
{

}

void AnimBaseButton::selected(bool var)
{
	SimpleButton::selected(var);
}


AnimRotateButton* AnimRotateButton::createWithSpriteFrameName(const char *normalName, const char *selectedName, const char *disabledName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector)
{
	AnimRotateButton *pRet = new AnimRotateButton();
	if (pRet)
	{
		cocos2d::Sprite* normalSprite = nullptr;
		cocos2d::Sprite* selectedSprite = nullptr;
		cocos2d::Sprite* disabledSprite = nullptr;
		if (normalName && strcmp(normalName, "") != 0)
			normalSprite = cocos2d::Sprite::createWithSpriteFrameName(normalName);
		if (selectedName && strcmp(selectedName, "") != 0)
			selectedSprite = cocos2d::Sprite::createWithSpriteFrameName(selectedName);
		if (disabledName && strcmp(disabledName, "") != 0)
			disabledSprite = cocos2d::Sprite::createWithSpriteFrameName(disabledName);
		if (pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, target, selector))
		{
			pRet->autorelease();
			return pRet;
		}
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void AnimRotateButton::setRotateImage(cocos2d::Sprite* var, int tag)
{
	if (m_pAnimNode)
	{
		removeChild(m_pAnimNode, true);
		m_pAnimNode = nullptr;
	}
	if (var)
	{
		m_pAnimNode = var;
		m_pAnimNode->setPosition(cocos2d::Point(getContentSize().width / 2, getContentSize().height / 2));
		addChild(m_pAnimNode, tag);
	}
}

void AnimRotateButton::startAnim()
{
	stopAnim();
	if (m_pAnimNode)
	{
		float xxx = m_pAnimNode->getRotation();
		cocos2d::RotateBy* actionby = cocos2d::RotateBy::create(m_rotateDur, 360);
		if (m_isForever)
		{
			m_pAnimNode->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(actionby, nullptr)));
		}
		else
		{
			m_pAnimNode->runAction(cocos2d::Sequence::create(actionby, nullptr));
		}
	}
}

void AnimRotateButton::stopAnim()
{
	if (m_pAnimNode)
	{
		m_pAnimNode->stopAllActions();
		m_pAnimNode->setRotation(0);
	}
}

AnimRotateButton::AnimRotateButton(void)
{
	m_rotateDur = 0.5f;
}

void AnimRotateButton::selected(bool var)
{
	AnimBaseButton::selected(var);
	if (var)
		startAnim();
	// 	else
	// 		stopAnim();
}
