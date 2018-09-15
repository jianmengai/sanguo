#pragma once

#include "SimpleButton.h"

//	带动画的按钮
class AnimBaseButton : public SimpleButton
{
public:
	AnimBaseButton(void);
	virtual ~AnimBaseButton(void);

	//响应
	virtual void selected(bool var);

	virtual void startAnim() = 0;
	virtual void stopAnim() = 0;

	//是否永久播放
	CC_SYNTHESIZE(bool, m_isForever, IsForever);

protected:
	//播放动画的节点
	cocos2d::Sprite* m_pAnimNode;
};


//	带旋转图片的button
class AnimRotateButton : public AnimBaseButton
{
public:
	static AnimRotateButton* createWithSpriteFrameName(const char *normalName, const char *selectedName, const char *disabledName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

	AnimRotateButton(void);
	//super
	virtual void selected(bool var);

	virtual void startAnim();
	virtual void stopAnim();

	//设置要旋转的图片
	void setRotateImage(cocos2d::Sprite* var, int tag = -1);
	//旋转一次时间
	CC_SYNTHESIZE(float, m_rotateDur, RotateDuration);
};
