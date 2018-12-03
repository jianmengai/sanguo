#pragma once

#include "cocos2d.h"


class PopupLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene * scene();
	bool init();
	CREATE_FUNC(PopupLayer);
private:
	//ע�ᴥ���¼���ʵ��ccTouchBegan()����
	//void registerWithTouchDispatcher();
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	//void OkButton(CCObject * object);
	//���öԻ����title
	void setTitle();
	//���öԻ�����ı�����
	void setContent();
private:
	//m_size������ǶԻ��򱳾��Ĵ�С
	cocos2d::Size m_size;
	//�Ի���ı�������
	cocos2d::Sprite * m_bgSprite;
};