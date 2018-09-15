#include "AccountLayer.h"
#include "UIDefine.h"
#include "GameDefine.h"

AccountLayer::AccountLayer()
{

}

AccountLayer::~AccountLayer()
{

}

bool AccountLayer::init()
{
	setVisible(false);
	return true;
}

void AccountLayer::initPanel()
{
	PlistManager::getInstance().addUISpriteFrames(LOGIN_PLIST);
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	//±³¾°Í¼
	cocos2d::Sprite* logBG = cocos2d::Sprite::create(LOGIN_BG);
	logBG->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
	this->addChild(logBG);

	int startX = 0;
	int startY = 30;
	int gapY = 75;
	cocos2d::Sprite* desSprite = nullptr;
	cocos2d::Point desPos;
	//ÕËºÅÊäÈë¿ò±³¾°
	cocos2d::Sprite* pTextUserFieldBg = cocos2d::Sprite::createWithSpriteFrameName(LOGIN_INPUT);
	desPos = cocos2d::Point(winSize.width / 2, winSize.height / 2 + pTextUserFieldBg->getContentSize().height * 2);
	pTextUserFieldBg->setPosition(desPos);
	this->addChild(pTextUserFieldBg);
	//ÓÃ»§ÃûÎÄ×ÖÍ¼Æ¬
	desSprite = cocos2d::Sprite::createWithSpriteFrameName(LOGIN_ACCOUNT_DES);
	desSprite->setAnchorPoint(cocos2d::Point(1.0f, 0.5f));
	desPos = cocos2d::Point(-20, pTextUserFieldBg->getContentSize().height / 2);
	desSprite->setPosition(desPos);
	pTextUserFieldBg->addChild(desSprite);

	//ÃÜÂëÊäÈë¿ò±³¾°
	cocos2d::Sprite* pTextPasswordFieldBg = cocos2d::Sprite::createWithSpriteFrameName(LOGIN_INPUT);
	desPos = cocos2d::Point(winSize.width / 2, winSize.height / 2);
	pTextPasswordFieldBg->setPosition(desPos);
	this->addChild(pTextPasswordFieldBg);
	//ÃÜÂëÎÄ×ÖÍ¼Æ¬
	desSprite = cocos2d::Sprite::createWithSpriteFrameName(LOGIN_PWD_DES);
	desSprite->setAnchorPoint(cocos2d::Point(1.0f, 0.5f));
	desPos = cocos2d::Point(-20, pTextUserFieldBg->getContentSize().height / 2);
	desSprite->setPosition(desPos);
	pTextPasswordFieldBg->addChild(desSprite);
	
	//µÇÂ½°´Å¥
	/*AnimRotateButton *pLoginItem = AnimRotateButton::createWithSpriteFrameName(
		LOGIN_NORMAL,
		LOGIN_SELECTED,
		nullptr,
		this,
		CC_MENU_SELECTOR(AccountLayer::menuLoginCallback));
	pLoginItem->setRotateImage(cocos2d::Sprite::createWithSpriteFrameName(LOGIN_ROTATE_BIG));
	pLoginItem->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2 - pLoginItem->getContentSize().height));
	m_pLoginMenu = Menu::create(pLoginItem, nullptr);
	m_pLoginMenu->setPosition(cocos2d::Point(0, 0));
	this->addChild(m_pLoginMenu, 1);*/
	
	setTouchEnabled(true);

	setVisible(true);

	cocos2d::ui::Button* pLoginButton = cocos2d::ui::Button::create();
	pLoginButton->setTouchEnabled(true);
	pLoginButton->loadTextures(LOGIN_NORMAL, LOGIN_SELECTED, "", cocos2d::ui::Widget::TextureResType::PLIST);
	pLoginButton->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2 - 50));
	pLoginButton->addTouchEventListener(CC_CALLBACK_2(AccountLayer::buttonLoginCallback, this));
	this->addChild(pLoginButton);

}

void AccountLayer::onEnter()
{

	//ÕËºÅÊäÈë¿ò
	cocos2d::Size editBoxSize = cocos2d::Size(312, 60);
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	int startX = 4;
	int startY = 28;
	int gapY = 75;
	
	m_pTextUserName = TextInput::create(editBoxSize, cocos2d::ui::Scale9Sprite::createWithSpriteFrameName(BG_TRANSPARENT_42_42, BG_TRANSPARENT_42_42_CCRect));
	m_pTextUserName->setAnchorPoint(cocos2d::Point());
	m_pTextUserName->setPosition(winSize.width / 2, winSize.height / 2 + m_pTextUserName->getContentSize().height * 2);
	m_pTextUserName->setFontName(FONT_NAME_DEFAULT);
	m_pTextUserName->setFontSize(FONT_SIZE_20);
	m_pTextUserName->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	m_pTextUserName->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	m_pTextUserName->setFontColor(CH_GREEN);
	m_pTextUserName->setMaxLength(20);
	m_pTextUserName->setPlaceHolder("");
	m_pTextUserName->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::GO);
	m_pTextUserName->setDelegate(this);
	m_pTextUserName->setText("username"/*cocos2d::UserDefault::getInstance()->getStringForKey(LAST_ACCOUNT_NAME).c_str()*/);
	this->addChild(m_pTextUserName, 50);

	//ÃÜÂëÊäÈë¿ò
	m_pTextPassword = TextInput::create(editBoxSize, cocos2d::ui::Scale9Sprite::createWithSpriteFrameName(BG_TRANSPARENT_42_42, BG_TRANSPARENT_42_42_CCRect));
	m_pTextPassword->setPosition(startX, startY - gapY);
	m_pTextPassword->setFontName(FONT_NAME_DEFAULT);
	m_pTextPassword->setFontSize(FONT_SIZE_20);
	m_pTextPassword->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
	m_pTextPassword->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	m_pTextPassword->setFontColor(CH_GREEN);
	m_pTextPassword->setMaxLength(20);
	m_pTextPassword->setPlaceHolder("");
	m_pTextPassword->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::GO);
	m_pTextPassword->setDelegate(this);
	m_pTextPassword->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
	m_pTextPassword->setText("password"/*cocos2d::UserDefault::getInstance()->getStringForKey(LAST_PASSWORD).c_str()*/);
	this->addChild(m_pTextPassword, 50);

	cocos2d::Layer::onEnter();
}

void AccountLayer::onExit()
{
	cocos2d::Layer::onExit();
	PlistManager::getInstance().removeUISpirteFrames(LOGIN_PLIST);
}

std::string AccountLayer::GetUserText()
{
	std::string name = m_pTextUserName->getText();
	if (name == "")
	{
		name = m_pTextUserName->getPlaceHolder();
	}
	return name;
}

std::string AccountLayer::GetPasswordText()
{
	std::string pass = m_pTextPassword->getText();
	if (pass == "")
	{
		pass = m_pTextPassword->getPlaceHolder();
	}
	return pass;
}


void AccountLayer::fadeOutCallback()
{
	cocos2d::Node * child = this->getChildByTag(100);
	this->removeChild(child, true);
}

void AccountLayer::menuLoginCallback(cocos2d::Ref* pSender)
{
	std::string userName = GetUserText();
	std::string passWorld = GetPasswordText();

	if (userName == "" || passWorld == "")
	{
		std::string tip = "";
		if (userName == "")
		{
			tip = GETSTR("zhanghaoweikong");
		}
		else if (passWorld == "")
		{
			tip = GETSTR("mimaweikong");
		}
		
		auto winSize = cocos2d::Director::getInstance()->getWinSize();
		cocos2d::Label* errmsgLabel = cocos2d::Label::createWithTTF(tip, FONT_NAME_DEFAULT, FONT_SIZE_24);
		errmsgLabel->setPosition(cocos2d::Point(0, 0));  // AccountLaye µÄ×ø±êÔÚÆÁÄ»ÖÐ¼ä
		errmsgLabel->setColor(cocos2d::Color3B(255, 0, 255));
		errmsgLabel->setTag(100);
		this->addChild(errmsgLabel, 100);
		cocos2d::FadeOut* fadeOut = cocos2d::FadeOut::create(3.0f);
		cocos2d::CallFunc* callback = cocos2d::CallFunc::create(CC_CALLBACK_0(AccountLayer::fadeOutCallback, this));
		errmsgLabel->runAction(cocos2d::Sequence::create(fadeOut, callback, NULL));

		return;
	}

	//AddLoginPage();

	//LoginController::getInstance().setUsername(userName);

	//LoginController::getInstance().setPassword(passWorld);


	//GameGobal::getInstance().getLoginLayer()->showLayer(login_layer_type_server);
}

void AccountLayer::buttonLoginCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		cocos2d::log("began...");
		break;

	case cocos2d::ui::Widget::TouchEventType::MOVED:
		cocos2d::log("moved...");
		break;

	case cocos2d::ui::Widget::TouchEventType::ENDED:
		cocos2d::log("ended...");
		break;

	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		cocos2d::log("canceled...");
		break;

	default:
		break;
	}

}
