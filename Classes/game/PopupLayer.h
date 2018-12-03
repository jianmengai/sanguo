#pragma once

#include "cocos2d.h"


class PopupLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene * scene();
	bool init();
	CREATE_FUNC(PopupLayer);
private:
	//注册触摸事件，实现ccTouchBegan()方法
	//void registerWithTouchDispatcher();
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	//void OkButton(CCObject * object);
	//设置对话框的title
	void setTitle();
	//设置对话框的文本内容
	void setContent();
private:
	//m_size代表的是对话框背景的大小
	cocos2d::Size m_size;
	//对话框的背景精灵
	cocos2d::Sprite * m_bgSprite;
};