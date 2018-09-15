#pragma once

#include "BaseNode.h"
#include "UIProtocol.h"




class SimpleButton : public BaseNode, public ResponseProtocol
{
public:
	SimpleButton(void);
	~SimpleButton(void);

	static void setDefaultBtnStringTnf(std::string var);//设置字体文件（当选用CCLabelBMFont时有用）
	static void setDefaultBtnFontType(int var);//设置字体类型

	//根据noramlNode\selectedNode\disabledNode创建一个checkBtn
	static SimpleButton* createWithNode(cocos2d::Node* noramlNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, const char* btnStr = "");
	static SimpleButton* createWithNode(cocos2d::Node* noramlNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector, const char* btnStr = "");

	static SimpleButton* create(const char *normalImage, const char *selectedImage, cocos2d::Object* target = nullptr, cocos2d::SEL_MenuHandler selector = nullptr);
	static SimpleButton* create(const char *normalImage, const char *selectedImage, const char *disabledImage, cocos2d::Object* target = nullptr, cocos2d::SEL_MenuHandler selector = nullptr);

	static SimpleButton* createWithSpriteFrame(cocos2d::SpriteFrame *normalSpriteFrame, cocos2d::SpriteFrame *selectedSpriteFrame, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector);
	static SimpleButton* createWithSpriteFrameName(const char *normalName, const char *selectedName, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector);
	static SimpleButton* createWithSpriteFrameName(const char *normalName, const char *selectedName, const char *disabledName, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector);

	bool initWithNode(cocos2d::Node* normalNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector, const char* btnStr = "");

	/////////////////////////////////字/////////////////////////////////////////
	void setString(const char* str);//设置按钮上的字
	void setFontColor(const cocos2d::Color3B& color);//设置字体颜色
	void setFontSize(float fontSize);//设置字体
	void setFontName(const char *fontName);//设置字体大小
	const char* getString();//获取按钮上的字

	/////////////////////////////////响应/////////////////////////////////////////
	virtual void activate();
	virtual void selected(bool var);
	virtual bool isEnabled();
	virtual void setEnabled(bool value);
	virtual bool isSelected();

	////////////////////////////////////消息//////////////////////////////////////
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event *pEvent);
	virtual void onTouchMoved(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	virtual bool onTouchEnded(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	virtual void onTouchCancelled(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	//是否点中
	virtual bool onTouch(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);

	CC_PROPERTY(cocos2d::Node*, m_pNormalImage, NormalImage);
	CC_PROPERTY(cocos2d::Node*, m_pSelectedImage, SelectedImage);
	CC_PROPERTY(cocos2d::Node*, m_pDisabledImage, DisabledImage);

	//显示文字
	CC_SYNTHESIZE_READONLY(cocos2d::Node*, m_label, Label);
protected:
	virtual void updateImagesVisibility();
protected:
	bool m_bSelected;
	bool m_bEnabled;
};
