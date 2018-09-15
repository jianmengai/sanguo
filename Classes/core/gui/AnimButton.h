#pragma once

#include "SimpleButton.h"

//	�������İ�ť
class AnimBaseButton : public SimpleButton
{
public:
	AnimBaseButton(void);
	virtual ~AnimBaseButton(void);

	//��Ӧ
	virtual void selected(bool var);

	virtual void startAnim() = 0;
	virtual void stopAnim() = 0;

	//�Ƿ����ò���
	CC_SYNTHESIZE(bool, m_isForever, IsForever);

protected:
	//���Ŷ����Ľڵ�
	cocos2d::Sprite* m_pAnimNode;
};


//	����תͼƬ��button
class AnimRotateButton : public AnimBaseButton
{
public:
	static AnimRotateButton* createWithSpriteFrameName(const char *normalName, const char *selectedName, const char *disabledName, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler selector);

	AnimRotateButton(void);
	//super
	virtual void selected(bool var);

	virtual void startAnim();
	virtual void stopAnim();

	//����Ҫ��ת��ͼƬ
	void setRotateImage(cocos2d::Sprite* var, int tag = -1);
	//��תһ��ʱ��
	CC_SYNTHESIZE(float, m_rotateDur, RotateDuration);
};
