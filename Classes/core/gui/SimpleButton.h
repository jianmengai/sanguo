#pragma once

#include "BaseNode.h"
#include "UIProtocol.h"




class SimpleButton : public BaseNode, public ResponseProtocol
{
public:
	SimpleButton(void);
	~SimpleButton(void);

	static void setDefaultBtnStringTnf(std::string var);//���������ļ�����ѡ��CCLabelBMFontʱ���ã�
	static void setDefaultBtnFontType(int var);//������������

	//����noramlNode\selectedNode\disabledNode����һ��checkBtn
	static SimpleButton* createWithNode(cocos2d::Node* noramlNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, const char* btnStr = "");
	static SimpleButton* createWithNode(cocos2d::Node* noramlNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector, const char* btnStr = "");

	static SimpleButton* create(const char *normalImage, const char *selectedImage, cocos2d::Object* target = nullptr, cocos2d::SEL_MenuHandler selector = nullptr);
	static SimpleButton* create(const char *normalImage, const char *selectedImage, const char *disabledImage, cocos2d::Object* target = nullptr, cocos2d::SEL_MenuHandler selector = nullptr);

	static SimpleButton* createWithSpriteFrame(cocos2d::SpriteFrame *normalSpriteFrame, cocos2d::SpriteFrame *selectedSpriteFrame, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector);
	static SimpleButton* createWithSpriteFrameName(const char *normalName, const char *selectedName, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector);
	static SimpleButton* createWithSpriteFrameName(const char *normalName, const char *selectedName, const char *disabledName, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector);

	bool initWithNode(cocos2d::Node* normalNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, cocos2d::Object* target, cocos2d::SEL_MenuHandler selector, const char* btnStr = "");

	/////////////////////////////////��/////////////////////////////////////////
	void setString(const char* str);//���ð�ť�ϵ���
	void setFontColor(const cocos2d::Color3B& color);//����������ɫ
	void setFontSize(float fontSize);//��������
	void setFontName(const char *fontName);//���������С
	const char* getString();//��ȡ��ť�ϵ���

	/////////////////////////////////��Ӧ/////////////////////////////////////////
	virtual void activate();
	virtual void selected(bool var);
	virtual bool isEnabled();
	virtual void setEnabled(bool value);
	virtual bool isSelected();

	////////////////////////////////////��Ϣ//////////////////////////////////////
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event *pEvent);
	virtual void onTouchMoved(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	virtual bool onTouchEnded(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	virtual void onTouchCancelled(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	//�Ƿ����
	virtual bool onTouch(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);

	CC_PROPERTY(cocos2d::Node*, m_pNormalImage, NormalImage);
	CC_PROPERTY(cocos2d::Node*, m_pSelectedImage, SelectedImage);
	CC_PROPERTY(cocos2d::Node*, m_pDisabledImage, DisabledImage);

	//��ʾ����
	CC_SYNTHESIZE_READONLY(cocos2d::Node*, m_label, Label);
protected:
	virtual void updateImagesVisibility();
protected:
	bool m_bSelected;
	bool m_bEnabled;
};
