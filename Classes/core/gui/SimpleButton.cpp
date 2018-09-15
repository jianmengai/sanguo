#include "SimpleButton.h"
#include "CoreDefine.h"

static std::string g_defaultBtnStringTnf = ""; //按钮上文字对应的tnf
static int g_defaultBtnFontType = 1;          //1:CCLabelTTF, 2:CCLabelBMFont

SimpleButton::SimpleButton(void)
{
	m_pNormalImage = nullptr;
	m_pSelectedImage = nullptr;
	m_pDisabledImage = nullptr;
	m_pListener = nullptr;
	m_pfnSelector = nullptr;
	setAnchorPoint(cocos2d::Point(.5f, .5f));
	m_label = nullptr;//改后加
}
SimpleButton::~SimpleButton(void)
{
}

SimpleButton* SimpleButton::createWithNode(cocos2d::Node* noramlNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, cocos2d::Object* target, cocos2d::SEL_CallFuncO selector, const char* btnStr/* = */)
{
	SimpleButton *pRet = new SimpleButton();
	if (pRet && pRet->initWithNode(noramlNode, selectedNode, disabledNode, target, selector, btnStr))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

SimpleButton* SimpleButton::createWithNode(cocos2d::Node* noramlNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, const char* btnStr/*=""*/)
{
	return SimpleButton::createWithNode(noramlNode, selectedNode, disabledNode, nullptr, nullptr, btnStr);
}

SimpleButton * SimpleButton::create(const char *normalImage, const char *selectedImage, cocos2d::Object* target, cocos2d::SEL_CallFuncO selector)
{
	return SimpleButton::create(normalImage, selectedImage, nullptr, target, selector);
}
SimpleButton * SimpleButton::create(const char *normalImage, const char *selectedImage, const char *disabledImage, cocos2d::Object* target, cocos2d::SEL_CallFuncO selector)
{
	SimpleButton *pRet = new SimpleButton();
	cocos2d::Node *normalSprite = nullptr;
	cocos2d::Node *selectedSprite = nullptr;
	cocos2d::Node *disabledSprite = nullptr;

	if (normalImage)
	{
		normalSprite = cocos2d::Sprite::create(normalImage);
	}
	if (selectedImage)
	{
		selectedSprite = cocos2d::Sprite::create(selectedImage);
	}
	if (disabledImage)
	{
		disabledSprite = cocos2d::Sprite::create(disabledImage);
	}
	if (pRet && pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, target, selector))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}
SimpleButton* SimpleButton::createWithSpriteFrame(cocos2d::SpriteFrame *normalSpriteFrame, cocos2d::SpriteFrame *selectedSpriteFrame, cocos2d::Object* target, cocos2d::SEL_CallFuncO selector)
{
	SimpleButton *pRet = new SimpleButton();
	if (pRet)
	{
		cocos2d::Sprite* normalSprite = cocos2d::Sprite::createWithSpriteFrame(normalSpriteFrame);
		cocos2d::Sprite* selectedSprite = cocos2d::Sprite::createWithSpriteFrame(selectedSpriteFrame);
		cocos2d::Sprite* disabledSprite = nullptr;
		if (pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, target, selector))
		{
			pRet->autorelease();
			return pRet;
		}
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}
SimpleButton* SimpleButton::createWithSpriteFrameName(const char *normalName, const char *selectedName, cocos2d::Object* target, cocos2d::SEL_CallFuncO selector)
{
	SimpleButton *pRet = new SimpleButton();
	if (pRet)
	{
		cocos2d::Sprite* normalSprite = cocos2d::Sprite::createWithSpriteFrameName(normalName);
		cocos2d::Sprite* selectedSprite = cocos2d::Sprite::createWithSpriteFrameName(selectedName);
		cocos2d::Sprite* disabledSprite = nullptr;
		if (pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, target, selector))
		{
			pRet->autorelease();
			return pRet;
		}
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}
SimpleButton* SimpleButton::createWithSpriteFrameName(const char *normalName, const char *selectedName, const char *disabledName, cocos2d::Object* target, cocos2d::SEL_CallFuncO selector)
{
	SimpleButton *pRet = new SimpleButton();
	if (pRet)
	{
		cocos2d::Sprite* normalSprite = cocos2d::Sprite::createWithSpriteFrameName(normalName);
		cocos2d::Sprite* selectedSprite = cocos2d::Sprite::createWithSpriteFrameName(selectedName);
		cocos2d::Sprite* disabledSprite = cocos2d::Sprite::createWithSpriteFrameName(disabledName);;
		if (pRet->initWithNode(normalSprite, selectedSprite, disabledSprite, target, selector))
		{
			pRet->autorelease();
			return pRet;
		}
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}
bool SimpleButton::initWithNode(cocos2d::Node* normalNode, cocos2d::Node* selectedNode, cocos2d::Node* disabledNode, cocos2d::Object* target, cocos2d::SEL_CallFuncO selector, const char* btnStr/* = */)
{
	setTarget(target, selector);
	m_bEnabled = true;
	m_bSelected = false;
	setNormalImage(normalNode);
	setSelectedImage(selectedNode);
	setDisabledImage(disabledNode);

	if (g_defaultBtnFontType == 2)
	{
		m_label = cocos2d::LabelBMFont::create(btnStr, g_defaultBtnStringTnf.c_str());
	}
	else
	{
		m_label = cocos2d::LabelTTF::create(btnStr, FONT_NAME_HELVETICA, FONT_SIZE_24);
	}
	m_label->setPosition(cocos2d::Point(getContentSize().width / 2, getContentSize().height / 2));
	setFontColor(cocos2d::Color3B(255,255,255));
	addChild(m_label, 100);

	return true;
}
void SimpleButton::setString(const char* str)
{
	if (m_label)
	{
		cocos2d::LabelBMFont* bmFont = dynamic_cast<cocos2d::LabelBMFont*>(m_label);
		if (bmFont)
		{
			bmFont->setString(str);
		}
		else
		{
			cocos2d::LabelTTF* ttf = dynamic_cast<cocos2d::LabelTTF*>(m_label);
			if (ttf)
			{
				ttf->setString(str);
			}
		}
		if (0 == strcmp(str, ""))
		{
			m_label->setVisible(false);
		}
		else
		{
			m_label->setVisible(true);
		}
	}
}
void SimpleButton::setFontColor(const cocos2d::Color3B& color)
{
	cocos2d::LabelBMFont* bmFont = dynamic_cast<cocos2d::LabelBMFont*>(m_label);
	if (bmFont)
	{
		bmFont->setColor(color);
	}
	else
	{
		cocos2d::LabelTTF* ttf = dynamic_cast<cocos2d::LabelTTF*>(m_label);
		if (ttf)
		{
			ttf->setColor(color);
		}
	}
}
void SimpleButton::setFontSize(float fontSize)
{
	cocos2d::LabelBMFont* bmFont = dynamic_cast<cocos2d::LabelBMFont*>(m_label);
	if (bmFont)
	{
		float scale = fontSize / 24;
		bmFont->setScale(scale);
	}
	else
	{
		cocos2d::LabelTTF* ttf = dynamic_cast<cocos2d::LabelTTF*>(m_label);
		if (ttf)
		{
			ttf->setFontSize(fontSize);
		}
	}
}
void SimpleButton::setFontName(const char* fontName)
{
	cocos2d::LabelTTF* ttf = dynamic_cast<cocos2d::LabelTTF*>(m_label);
	if (ttf)
	{
		ttf->setFontName(fontName);
	}
}
const char* SimpleButton::getString()
{
	cocos2d::LabelBMFont* bmFont = dynamic_cast<cocos2d::LabelBMFont*>(m_label);
	if (bmFont)
	{
		return bmFont->getString().c_str();
	}
	else
	{
		cocos2d::LabelTTF* ttf = dynamic_cast<cocos2d::LabelTTF*>(m_label);
		if (ttf)
		{
			return ttf->getString().c_str();
		}
	}
	return "";
}

cocos2d::Node * SimpleButton::getNormalImage() const
{
	return m_pNormalImage;
}

void SimpleButton::setNormalImage(Node* pImage)
{
	if (pImage != m_pNormalImage)
	{
		if (pImage)
		{
			addChild(pImage);
			pImage->setAnchorPoint(cocos2d::Point(0, 0));
		}

		if (m_pNormalImage)
		{
			removeChild(m_pNormalImage, true);
		}

		m_pNormalImage = pImage;
		this->setContentSize(m_pNormalImage->getContentSize());
		this->updateImagesVisibility();
	}
}

cocos2d::Node * SimpleButton::getSelectedImage() const
{
	return m_pSelectedImage;
}

void SimpleButton::setSelectedImage(Node* pImage)
{
	if (pImage != m_pNormalImage)
	{
		if (pImage)
		{
			addChild(pImage);
			pImage->setAnchorPoint(cocos2d::Point(0, 0));
		}

		if (m_pSelectedImage)
		{
			removeChild(m_pSelectedImage, true);
		}

		m_pSelectedImage = pImage;
		this->updateImagesVisibility();
	}
}

cocos2d::Node * SimpleButton::getDisabledImage() const
{
	return m_pDisabledImage;
}

void SimpleButton::setDisabledImage(cocos2d::Node* pImage)
{
	if (pImage != m_pNormalImage)
	{
		if (pImage)
		{
			addChild(pImage);
			pImage->setAnchorPoint(cocos2d::Point(0, 0));
		}

		if (m_pDisabledImage)
		{
			removeChild(m_pDisabledImage, true);
		}

		m_pDisabledImage = pImage;
		this->updateImagesVisibility();
	}
}

void SimpleButton::setDefaultBtnStringTnf(std::string var)
{
	g_defaultBtnStringTnf = var;
}
void SimpleButton::setDefaultBtnFontType(int var)
{
	g_defaultBtnFontType = var;
}

void SimpleButton::updateImagesVisibility()
{
	if (m_bEnabled)
	{
		if (m_bSelected)
		{
			if (m_pNormalImage)   m_pNormalImage->setVisible(false);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(true);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
		}
		else
		{
			if (m_pNormalImage)   m_pNormalImage->setVisible(true);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
		}
	}
	else
	{
		if (m_pDisabledImage)
		{
			if (m_pNormalImage)   m_pNormalImage->setVisible(false);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(true);
		}
		else
		{
			if (m_pNormalImage)   m_pNormalImage->setVisible(true);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
		}
	}
}


void SimpleButton::activate()
{
	if (m_bEnabled)
	{
		if (m_pListener && m_pfnSelector)
		{
			(m_pListener->*m_pfnSelector)(this);
		}
	}
}

void SimpleButton::setEnabled(bool enabled)
{
	if (m_bEnabled != enabled)
	{
		m_bEnabled = enabled;
		updateImagesVisibility();
	}
}

bool SimpleButton::isEnabled()
{
	return m_bEnabled;
}

bool SimpleButton::isSelected()
{
	return m_bSelected;
}

void SimpleButton::selected(bool var)
{
	m_bSelected = var;
	updateImagesVisibility();
}

bool SimpleButton::onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event *pEvent)
{
	if (!isEnabled() || !isShow())
	{
		return false;
	}
	if (onTouch(pTouch, pEvent))
	{
		selected(true);
		return true;
	}
	return false;
}

void SimpleButton::onTouchMoved(cocos2d::Touch*pTouch, cocos2d::Event*pEvent)
{

}

bool SimpleButton::onTouchEnded(cocos2d::Touch*pTouch, cocos2d::Event*pEvent)
{
	if (m_bEnabled && isShow())
	{
		selected(false);
		if (onTouch(pTouch, pEvent))
		{
			activate();
			return true;
		}
	}
	return false;
}

void SimpleButton::onTouchCancelled(cocos2d::Touch*pTouch, cocos2d::Event*pEvent)
{
	if (isShow())
	{
		selected(false);
	}
}

bool SimpleButton::onTouch(cocos2d::Touch*pTouch, cocos2d::Event*pEvent)
{
	cocos2d::Point local = convertTouchToNodeSpace(pTouch);
	cocos2d::Rect r = rect();
	r.origin = cocos2d::Point(0.0f, 0.0f);
	if (r.containsPoint(local))
	{
		return true;
	}
	return false;
}