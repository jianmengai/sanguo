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
	const char* getText();          //取得键盘输入字符串
	void setText(const char* str);  //设置键盘输入字符
	void setPlaceHolder(const char*  pText);    //提示信息 （但输入的信息为空时就显示）PlaceHolder和输入的内容共用一个cclabelTTF
	const char * getPlaceHolder();
	cocos2d::Color3B getColorSpaceHolder();
	void setColorSpaceHolder(const cocos2d::Color3B& color);

	void clearText();               //清空键盘输入字符

	void setFontColor(cocos2d::Color3B color);         //TextField 颜色
	void setFontName(const char*  pFontName);   //默认 FONT_NAME_DEFAULT
	void setFontSize(int fontSize);             //默认 FONT_SIZE_20
	void setFont(const char* pFontName, int fontSize);
	void setPlaceholderFontName(const char*  pFontName);  // 暂时没用，为了将来换成cceditbox 保留
	void setPlaceholderFontSize(int fontSize);            // 暂时没用，为了将来换成cceditbox 保留
	void setPlaceholderFont(const char* pFontName, int fontSize);
	void setMaxLength(int maxLength);           //最大输入字符
	void setReturnType(cocos2d::ui::EditBox::KeyboardReturnType returnType);  //暂时没用 cceditbox 的方法，为了将来换成cceditbox 保留
	void setInputMode(cocos2d::ui::EditBox::InputMode inputMode);
	void setInputFlag(cocos2d::ui::EditBox::InputFlag inputFlag);
	void setDelegate(void* pDelegate);
	void setHorizontalAlignment(cocos2d::TextHAlignment alignment);
	void setVerticalAlignment(cocos2d::TextVAlignment verticalAlignment);
	void setOnlyNum(bool isOnlyNum);  //是否只输入数字
	void setLabelEx(bool isLabelEx);    //聊天面板触发updateChatInput,只是聊天用!!!
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