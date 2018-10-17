#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

class TextInput : public cocos2d::Node, public cocos2d::IMEDelegate, public cocos2d::TextFieldDelegate, public cocos2d::ui::EditBoxDelegate
{

public:
	TextInput();
	~TextInput();

	//CREATE_FUNC(TextInput);
	static TextInput* create(cocos2d::Size editBoxSize, cocos2d::ui::Scale9Sprite * normalBg)
	{
		TextInput *pRet = new TextInput();
		pRet->setContentSize(editBoxSize);
		pRet->m_pNormalBg = normalBg;

		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return pRet;
		}
	}

	bool init();

public:
	const char* getText();          //ȡ�ü��������ַ���
	void setText(const char* str);  //���ü��������ַ�
	void setPlaceHolder(const char*  pText);    //��ʾ��Ϣ �����������ϢΪ��ʱ����ʾ��PlaceHolder����������ݹ���һ��cclabelTTF
	const char * getPlaceHolder();
	cocos2d::Color3B getColorSpaceHolder();
	void setColorSpaceHolder(const cocos2d::Color3B& color);

	void clearText();               //��ռ��������ַ�

	void setFontColor(cocos2d::Color3B color);         //TextField ��ɫ
	void setFontName(const char*  pFontName);   //Ĭ�� FONT_NAME_DEFAULT
	void setFontSize(int fontSize);             //Ĭ�� FONT_SIZE_20
	void setFont(const char* pFontName, int fontSize);
	void setPlaceholderFontName(const char*  pFontName);  // ��ʱû�ã�Ϊ�˽�������cceditbox ����
	void setPlaceholderFontSize(int fontSize);            // ��ʱû�ã�Ϊ�˽�������cceditbox ����
	void setPlaceholderFont(const char* pFontName, int fontSize);
	void setMaxLength(int maxLength);           //��������ַ�
	void setReturnType(cocos2d::ui::EditBox::KeyboardReturnType returnType);  //��ʱû�� cceditbox �ķ�����Ϊ�˽�������cceditbox ����
	void setInputMode(cocos2d::ui::EditBox::InputMode inputMode);
	void setInputFlag(cocos2d::ui::EditBox::InputFlag inputFlag);
	void setDelegate(void* pDelegate);
	void setHorizontalAlignment(cocos2d::TextHAlignment alignment);
	void setVerticalAlignment(cocos2d::TextVAlignment verticalAlignment);
	void setOnlyNum(bool isOnlyNum);  //�Ƿ�ֻ��������
	void setLabelEx(bool isLabelEx);    //������崥��updateChatInput,ֻ��������!!!
	void setVisible(bool isVisible);

	cocos2d::Rect getRect(cocos2d::Node * pNode);

public:
	bool onTouchBegan(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	void onTouchMoved(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	bool onTouchEnded(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
	void onTouchCancelled(cocos2d::Touch*pTouch, cocos2d::Event*pEvent);
public:
	void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info);  //CCIMEDelegate
	bool onTextFieldAttachWithIME(cocos2d::TextFieldTTF * pSender);
	bool onTextFieldDetachWithIME(cocos2d::TextFieldTTF * pSender);
	bool onTextFieldInsertText(cocos2d::TextFieldTTF * pSender, const char * text, int nLen);
	bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF * pSender, const char * delText, int nLen);
	bool onDraw(cocos2d::TextFieldTTF * pSender);

public:
	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	void editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, cocos2d::ui::EditBoxDelegate::EditBoxEndAction action);
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
	void editBoxReturn(cocos2d::ui::EditBox* editBox);
public:

public:
	cocos2d::ui::Scale9Sprite* m_pNormalBg;

	cocos2d::TextFieldTTF * m_pTextField;
	cocos2d::ui::EditBox * m_pEditBox;

	bool m_isOnlyNum;
	int m_nCharLimit; // the textfield max char limit
	cocos2d::Point m_beginPos;
	bool m_isLabelEx;

	float m_fAdjustHeight;

	cocos2d::EventListenerTouchOneByOne* m_pTouchListener;
};