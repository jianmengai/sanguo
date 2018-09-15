#include "SimpleAudioEngine.h"
#include "LoginScene.h"
#include "GameSceneManager.h"
#include "UIDefine.h"
#include "AccountLayer.h"

cocos2d::Scene* LoginLayer::scene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	LoginLayer *layer = LoginLayer::create();
	GameSceneManager::getInstance().setLoginLayer(layer);
	scene->addChild(layer);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/login.mp3");
	return scene;
}

LoginLayer::LoginLayer()
{
	m_pLoginBG = nullptr;
}

LoginLayer::~LoginLayer()
{

}

bool LoginLayer::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	return true;
}

void LoginLayer::onEnter()
{
	cocos2d::Layer::onEnter();
	initLoginPanel();
}
void LoginLayer::onExit()
{
	PlistManager::getInstance().removeUISpirteFrames(LOGIN_PLIST);
}

void LoginLayer::update(float dt)
{

}

void LoginLayer::initLoginPanel()
{
	PlistManager::getInstance().addUISpriteFrames(LOGIN_PLIST);
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	//±³¾°Í¼
	cocos2d::Sprite* logBG = cocos2d::Sprite::create(LOGIN_BG);
	logBG->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
	this->addChild(logBG);

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


	//ÕËºÅÊäÈë¿ò
	cocos2d::Size editBoxSize = cocos2d::Size(312, 60);
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
	m_pTextUserName->setText(cocos2d::UserDefault::getInstance()->getStringForKey(LAST_ACCOUNT_NAME).c_str());
	this->addChild(m_pTextUserName, 50);

	//ÃÜÂëÊäÈë¿ò
	m_pTextPassword = TextInput::create(editBoxSize, cocos2d::ui::Scale9Sprite::createWithSpriteFrameName(BG_TRANSPARENT_42_42, BG_TRANSPARENT_42_42_CCRect));
	m_pTextPassword->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
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
	m_pTextPassword->setText("hahahah"/*cocos2d::UserDefault::getInstance()->getStringForKey(LAST_PASSWORD).c_str()*/);
	this->addChild(m_pTextPassword);

	/*cocos2d::ui::EditBox* pEditBox = cocos2d::ui::EditBox::create(editBoxSize, cocos2d::ui::Scale9Sprite::createWithSpriteFrameName(BG_TRANSPARENT_42_42, BG_TRANSPARENT_42_42_CCRect));
	pEditBox->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
	pEditBox->setText("hahahahah ");
	this->addChild(pEditBox);*/

	cocos2d::TextFieldTTF* pTextTTF = cocos2d::TextFieldTTF::textFieldWithPlaceHolder("", m_pTextUserName->getContentSize(), cocos2d::TextHAlignment::LEFT, FONT_NAME_DEFAULT, FONT_SIZE_24);
	pTextTTF->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
	pTextTTF->setString("feufheufheu");
	this->addChild(pTextTTF);
	//µÇÂ¼°´Å¥
	cocos2d::ui::Button* pLoginButton = cocos2d::ui::Button::create();
	pLoginButton->setTouchEnabled(true);
	pLoginButton->loadTextures(LOGIN_NORMAL, LOGIN_SELECTED, "", cocos2d::ui::Widget::TextureResType::PLIST);
	pLoginButton->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2 - pTextPasswordFieldBg->getContentSize().height - 50));
	pLoginButton->addTouchEventListener(CC_CALLBACK_2(LoginLayer::buttonLoginCallback, this));
	this->addChild(pLoginButton);
}


void LoginLayer::buttonLoginCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
	{
		cocos2d::log("began...");
	}
	break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
	{
		cocos2d::log("moved...");
	}
	break;

	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		cocos2d::log("ended...");
		login();
	}
	break;

	case cocos2d::ui::Widget::TouchEventType::CANCELED:
	{
		cocos2d::log("canceled...");
	}
	break;

	default:
		break;
	}
}


std::string LoginLayer::GetUserText()
{
	std::string name = m_pTextUserName->getText();
	if (name == "")
	{
		name = m_pTextUserName->getPlaceHolder();
	}
	return name;
}

std::string LoginLayer::GetPasswordText()
{
	std::string pass = m_pTextPassword->getText();
	if (pass == "")
	{
		pass = m_pTextPassword->getPlaceHolder();
	}
	return pass;
}


void LoginLayer::login()
{
	std::string userName = GetUserText();
	std::string passWord = GetPasswordText();

	if (userName == "" || passWord == "")
	{
		std::string tip = "";
		if (userName == "")
		{
			tip = GETSTR("zhanghaoweikong");
		}
		else if (passWord == "")
		{
			tip = GETSTR("mimaweikong");
		}
		if (tip.empty())
		{
			tip = "username or password is empty";
		}
		auto winSize = cocos2d::Director::getInstance()->getWinSize();
		cocos2d::Label* errmsgLabel = cocos2d::Label::createWithTTF(tip, FONT_NAME_DEFAULT, FONT_SIZE_24);
		errmsgLabel->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
		errmsgLabel->setColor(cocos2d::Color3B(255, 0, 255));
		errmsgLabel->setTag(100);
		this->addChild(errmsgLabel, 100);
		cocos2d::FadeOut* fadeOut = cocos2d::FadeOut::create(3.0f);
		cocos2d::CallFunc* callback = cocos2d::CallFunc::create(CC_CALLBACK_0(LoginLayer::fadeOutCallback, this));
		errmsgLabel->runAction(cocos2d::Sequence::create(fadeOut, callback, NULL));

		return;
	}

	if (userName == "tester" && passWord == "1234")
	{
		std::string lastUserName = cocos2d::UserDefault::getInstance()->getStringForKey(LAST_ACCOUNT_NAME);
		std::string lastPassword = cocos2d::UserDefault::getInstance()->getStringForKey(LAST_PASSWORD);
		//¸üÐÂ±£´æµÄÓÃ»§ÃûÃÜÂë
		if (lastUserName != userName || lastPassword != passWord)
		{
			cocos2d::UserDefault::getInstance()->setStringForKey(LAST_ACCOUNT_NAME, userName);
			cocos2d::UserDefault::getInstance()->setStringForKey(LAST_PASSWORD, passWord);
		}

	}
}

void LoginLayer::fadeOutCallback()
{
	cocos2d::Node * child = this->getChildByTag(100);
	this->removeChild(child, true);
}
