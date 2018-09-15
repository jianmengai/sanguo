#include "MsgDisplay.h"
#include "UIDefine.h"

MsgDisplay::MsgDisplay()
{

}
MsgDisplay::~MsgDisplay()
{

}

bool MsgDisplay::init()
{
	bool retB = false;
	do
	{
		retB = true;
	} while (0);

	return retB;
}

void MsgDisplay::msgDisplay(const std::string& msg)
{
	this->setVisible(true);
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	m_pMsgLabel = cocos2d::Label::createWithTTF(msg, FONT_NAME_DEFAULT, FONT_SIZE_24);
	m_pMsgLabel->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
	m_pMsgLabel->setColor(cocos2d::Color3B(255, 0, 255));
	this->addChild(m_pMsgLabel);
	cocos2d::FadeOut* fadeOut = cocos2d::FadeOut::create(3.0f);
	cocos2d::CallFunc* callback = cocos2d::CallFunc::create(this, callfunc_selector(MsgDisplay::fadeOutCallback));
	m_pMsgLabel->runAction(cocos2d::Sequence::create(fadeOut, callback, nullptr));
}
void MsgDisplay::fadeOutCallback()
{
	this->removeChild(m_pMsgLabel, true);
	this->setVisible(false);
}