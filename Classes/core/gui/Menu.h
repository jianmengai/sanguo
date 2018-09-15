#pragma once

#include "BaseNode.h"

class SimpleButton;

class Menu : public BaseNode
{
	bool m_bEnabled;
public:
	Menu() : m_pSelectedItem(nullptr) {}
	virtual ~Menu() {}

	static Menu* create();
	static Menu* create(SimpleButton* item, ...);
	static Menu* createWithItem(SimpleButton* item);
	static Menu* createWithArray(cocos2d::Vector<SimpleButton*>& arrayOfItems);
	static Menu* createWithItems(SimpleButton *firstItem, va_list args);

	bool init();
	bool initWithArray(cocos2d::Vector<SimpleButton*>& arrayOfItems);

	void alignItemsVertically();
	void alignItemsVerticallyWithPadding(float padding);
	void alignItemsHorizontally();
	void alignItemsHorizontallyWithPadding(float padding);

	virtual void addChild(cocos2d::Node * child);
	virtual void addChild(cocos2d::Node * child, int zOrder);
	virtual void addChild(cocos2d::Node * child, int zOrder, int tag);
	virtual void removeChild(cocos2d::Node* child, bool cleanup);

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	virtual void onExit();

	virtual void setOpacityModifyRGB(bool bValue) { CC_UNUSED_PARAM(bValue); }
	virtual bool isOpacityModifyRGB(void) { return false; }

	virtual bool isEnabled() { return m_bEnabled; }
	virtual void setEnabled(bool value) { m_bEnabled = value; };

	CC_SYNTHESIZE(int, m_horizontallyAlignType, HorizontallyAlignType);//水平排版时align方式（居中、居左、居右）

protected:
	SimpleButton* itemForTouch(cocos2d::Touch * touch);
protected:
	SimpleButton *m_pSelectedItem;
};