#pragma once
#include "cocos2d.h"
#include "GameCore.h"

class AccountLayer;

class LoginLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* scene();

	LoginLayer();
	~LoginLayer();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void update(float dt);

	CREATE_FUNC(LoginLayer);

	virtual  void buttonLoginCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	virtual void fadeOutCallback();
private:
	void initLoginPanel();
	std::string GetUserText();     //获取登陆名字
	std::string GetPasswordText(); //获取登陆密码
	void login();
private:
	cocos2d::Sprite* m_pLoginBG;
	TextInput* m_pTextUserName;
	TextInput* m_pTextPassword;
};