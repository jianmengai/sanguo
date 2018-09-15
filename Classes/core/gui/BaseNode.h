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

	//��ʾ
	virtual void show(bool isShow);
	//�Ƿ���ʾ(������ڵ�unvisible����unvisible��false)
	virtual bool isShow();
	//�ռ�����rect
	virtual cocos2d::Rect rect();
	//���ûص�Ŀ��
	void setTarget(cocos2d::Object *rec, cocos2d::SEL_CallFuncO selector);

	/////////////////////////////////��Ϣ/////////////////////////////////////////
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event *pEvent);
	virtual void onTouchMoved(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	virtual bool onTouchEnded(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	virtual void onTouchCancelled(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);

protected:
	//�Ƿ�����ʾ��parent
	bool hasVisibleParents();
	//�ص�
	cocos2d::Object*       m_pListener;
	cocos2d::SEL_CallFuncO    m_pfnSelector;
};
