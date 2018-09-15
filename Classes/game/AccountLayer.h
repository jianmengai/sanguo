#pragma once

#include "cocos2d.h"
#include "GameCore.h"
class AccountLayer : public cocos2d::Layer
{
public:
	AccountLayer();
	~AccountLayer();

	CREATE_FUNC(AccountLayer);

	virtual bool init();
	virtual void initPanel();
	virtual void onEnter();
	virtual void onExit();
	virtual void menuLoginCallback(cocos2d::Ref* pSender);
	virtual void buttonLoginCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	virtual void fadeOutCallback();

	std::string GetUserText();     //获取登陆名字
	std::string GetPasswordText(); //获取登陆密码

private:
	TextInput* m_pTextUserName;
	TextInput* m_pTextPassword;
	Menu*      m_pLoginMenu;
};