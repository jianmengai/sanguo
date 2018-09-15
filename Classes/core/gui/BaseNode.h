#pragma once

#include "cocos2d.h"
#include "CoreDefine.h"

class NodeRGBA : public cocos2d::Node//, public cocos2d::RGBAProtocol
{
public:
	CREATE_FUNC(NodeRGBA);

	NodeRGBA();
	virtual ~NodeRGBA();

	virtual bool init();

	virtual GLubyte getOpacity() const;
	virtual GLubyte getDisplayedOpacity() const;
	virtual void setOpacity(GLubyte opacity);
	virtual void updateDisplayedOpacity(GLubyte parentOpacity);
	virtual bool isCascadeOpacityEnabled() const;
	virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);

	virtual const cocos2d::Color3B& getColor() const;
	virtual const cocos2d::Color3B& getDisplayedColor() const;
	virtual void setColor(const cocos2d::Color3B& color);
	virtual void updateDisplayedColor(const cocos2d::Color3B& parentColor);
	virtual bool isCascadeColorEnabled() const;
	virtual void setCascadeColorEnabled(bool cascadeColorEnabled);

	virtual void setOpacityModifyRGB(bool bValue) {}
	virtual bool isOpacityModifyRGB() const { return false; }
protected:
	GLubyte		_displayedOpacity, _realOpacity;
	cocos2d::Color3B	_displayedColor, _realColor;
	bool		_cascadeOpacityEnabled, _cascadeColorEnabled;
};

class BaseNode : public NodeRGBA
{
public:
	BaseNode(void);
	virtual ~BaseNode(void);

	//显示
	virtual void show(bool isShow);
	//是否显示(如果父节点unvisible或本身unvisible则false)
	virtual bool isShow();
	//空间所在rect
	virtual cocos2d::Rect rect();
	//设置回调目标
	void setTarget(cocos2d::Object *rec, cocos2d::SEL_CallFuncO selector);

	/////////////////////////////////消息/////////////////////////////////////////
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event *pEvent);
	virtual void onTouchMoved(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	virtual bool onTouchEnded(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	virtual void onTouchCancelled(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);

protected:
	//是否有显示的parent
	bool hasVisibleParents();
	//回调
	cocos2d::Object*       m_pListener;
	cocos2d::SEL_CallFuncO    m_pfnSelector;
};
