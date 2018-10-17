#include "TextInput.h"
#include "UIDefine.h"
#include "GameSceneManager.h"
#include "InternationManager.h"


TextInput::TextInput()
{
	m_pNormalBg = nullptr;
	m_pTextField = nullptr;
	m_pEditBox = nullptr;
	m_nCharLimit = 500;
	m_isOnlyNum = false;
	m_isLabelEx = false;
	m_fAdjustHeight = 0;
	m_pTouchListener = nullptr;
}

TextInput::~TextInput()
{

}

bool TextInput::init()
{
	bool bRet = false;
	do
	{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)    

		m_pEditBox = cocos2d::ui::EditBox::create(this->getContentSize(), m_pNormalBg);
		this->addChild(m_pEditBox);
#else
		m_pTextField = cocos2d::TextFieldTTF::textFieldWithPlaceHolder("", this->getContentSize(), cocos2d::TextHAlignment::LEFT, FONT_NAME_DEFAULT, FONT_SIZE_24);
		m_pTextField->setAnchorPoint(cocos2d::Point(0.0f, 0.0f));
		m_pTextField->setPosition(cocos2d::Point(10, 0));
		this->addChild(m_pTextField);
#endif

		bRet = true;
	} while (0);

	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	m_pTouchListener = cocos2d::EventListenerTouchOneByOne::create();
	m_pTouchListener->setSwallowTouches(true);
	m_pTouchListener->onTouchBegan = CC_CALLBACK_2(TextInput::onTouchBegan, this);
	m_pTouchListener->onTouchMoved = CC_CALLBACK_2(TextInput::onTouchMoved, this);
	m_pTouchListener->onTouchCancelled = CC_CALLBACK_2(TextInput::onTouchCancelled, this);
	m_pTouchListener->onTouchEnded = CC_CALLBACK_2(TextInput::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(m_pTouchListener, this);
	return bRet;
}

bool TextInput::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	
	if (!m_pTextField)
	{
		return false;
	} 

	auto pos = pTouch->getLocation();//this->convertToWorldSpace(this->convertTouchToNodeSpace(pTouch));
	auto bgRect = this->getBoundingBox();
	cocos2d::log("TextInput::onTouchBegan, this:%p, pox.x=%0.1f, pos.y=%0.1f, rect.x=%0.1f, rect.y=%0.1f, size.x=%0.1f, size.y=%0.1f\n", 
		this, pos.x, pos.y, bgRect.origin.x, bgRect.origin.y, bgRect.size.width, bgRect.size.height);
	//cocos2d::Rect bgRect; //= this->getRect(this);
	//cocos2d::Rect bgRect;
	//bgRect.origin = cocos2d::Point(-this->getContentSize().width / 2, -this->getContentSize().height / 2);
	//bgRect.size = this->getContentSize();

	if (bgRect.containsPoint(pos))
	{
		cocos2d::log("TextInput::onTouchBegan, this:%p, in rect", this);
		m_beginPos = pTouch->getLocation();
		return true;
	}
	return false;
}

bool TextInput::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	cocos2d::log("TextInput::onTouchEnded");
	if (!m_pTextField)
	{
		return false;
	}

	cocos2d::Point endPos = pTouch->getLocation();

	float delta = 5.0f;
	if (::abs(endPos.x - m_beginPos.x) > delta
		|| ::abs(endPos.y - m_beginPos.y) > delta)
	{
		m_beginPos.x = m_beginPos.y = 0;
		return false;  //滑动5个像素算没点中 
	}
	m_beginPos.x = m_beginPos.y = 0;


	//是不是点中 m_pTextField ？
	cocos2d::Point convertedLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(pTouch));
	//CCRect bgRect = m_pNormalBg->boundingBox();
	cocos2d::Rect bgRect = this->getBoundingBox();
	/*bgRect.origin = cocos2d::Point(-this->getContentSize().width / 2, -this->getContentSize().height / 2);
	bgRect.size = this->getContentSize();
*/
	if (bgRect.containsPoint(convertedLocation))
	{
		cocos2d::log("onTouchEnded, in rect, attach ime");
		//be clicked
		m_pTextField->attachWithIME();
		return true;
	}
	else
	{
		//not be clicked
		m_pTextField->detachWithIME();
	}

	return false;
}

void TextInput::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	cocos2d::log("TextInput::onTouchMoved");
}

void TextInput::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	cocos2d::log("TextInput::onTouchCancelled");
}


void TextInput::keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)
{

	//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)    
	//
	//	if (!m_pTextField)
	//	{
	//		return;
	//	}
	//	
	//	CCRect rectTracked = getRect(m_pTextField);
	//
	//	// some adjustment for margin between the keyboard and the edit box.
	//	rectTracked.origin.y -= 4;
	//
	//	// if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
	//	if (!rectTracked.intersectsRect(info.end))
	//	{
	//		return;
	//	}
	//
	//	m_fAdjustHeight = info.end.getMaxY() - rectTracked.getMinY();
	//	// CCLOG("CCEditBox:needAdjustVerticalPosition(%f)", m_fAdjustHeight);
	//
	//	//if (m_pEditBoxImpl != nullptr)
	//	//{
	//	//	m_pEditBoxImpl->doAnimationWhenKeyboardMove(info.duration, m_fAdjustHeight);
	//	//}
	//
	//#endif

}


//删除字符
bool TextInput::onTextFieldDeleteBackward(cocos2d::TextFieldTTF * pSender, const char * delText, int nLen)
{

	return false;
}

//输入字符
bool TextInput::onTextFieldInsertText(cocos2d::TextFieldTTF * pSender, const char * text, int nLen)
{

	// if insert enter, treat as default to detach with ime
	if (*text == '\n')
	{
		return false;
	}

	std::string tempStr = m_pTextField->getString();
	tempStr.append(text);
	if (tempStr.length() > m_nCharLimit)
	{
		m_pTextField->detachWithIME();
		return true;    //超过最大限制字数
	}

	if (m_isOnlyNum)  //是不是只输入数字
	{
		std::string::iterator itr;
		for (itr = tempStr.begin(); itr != tempStr.end(); itr++)
		{
			if (((*itr) < '0') || ((*itr) > '9'))
			{
				m_pTextField->detachWithIME();
				if (GameSceneManager::getInstance().getGameUILayer())
				{
					GameSceneManager::getInstance().getGameUILayer()->getMsgDisplay()->msgDisplay(GETSTR("buquanweishuzi"));
				}
				return true;    //超过最大限制字数
			}
		}
	}

	/*if (m_isLabelEx)
	{
		GameGobal::getInstance().getGameUILayer()->getChatPanel()->updateChatInput(tempStr);
	}*/
	return false;
}


//打开键盘
bool TextInput::onTextFieldAttachWithIME(cocos2d::TextFieldTTF * pSender)
{
	return false;
}

//关闭键盘
bool TextInput::onTextFieldDetachWithIME(cocos2d::TextFieldTTF * pSender)
{
	if (m_isLabelEx)
	{
		if (m_pTextField)
		{
			if (strcmp(m_pTextField->getString().c_str(), "") == 0)
			{
				m_pTextField->setVisible(true);  //显示m_pPlaceHolder
			}
			else
			{
				m_pTextField->setVisible(false); //显示m_pInputText
			}
		}
	}
	return false;
}

bool TextInput::onDraw(cocos2d::TextFieldTTF * pSender)
{
	return false;
}

const char* TextInput::getText()
{
	if (m_pTextField)
	{
		return m_pTextField->getString().c_str();
	}
	else if (m_pEditBox)
	{
		return m_pEditBox->getText();
	}

	return nullptr;
}

void TextInput::setText(const char* str)
{
	if (m_pTextField)
	{
		m_pTextField->setString(str);
		if (m_isLabelEx)
		{
			if (strcmp(str, "") == 0)
			{
				m_pTextField->setVisible(true);  //显示m_pPlaceHolder
			}
			else
			{
				m_pTextField->setVisible(false); //显示m_pInputText
			}
		}
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setText(str);
	}

}

const char * TextInput::getPlaceHolder()
{
	if (m_pTextField)
	{
		return m_pTextField->getPlaceHolder().c_str();
	}
	else if (m_pEditBox)
	{
		return m_pEditBox->getPlaceHolder();
	}

	return nullptr;

}
void TextInput::setPlaceHolder(const char* pText)
{
	if (m_pTextField)
	{
		m_pTextField->setPlaceHolder(pText);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setPlaceHolder(pText);
	}
}

void TextInput::clearText()
{
	if (m_pTextField)
	{
		m_pTextField->setString("");
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setText("");
	}

}

void TextInput::setFontColor(cocos2d::Color3B color)
{
	if (m_pTextField)
	{
		m_pTextField->setColor(color);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setFontColor(color);
	}
}

void TextInput::setFontName(const char* pFontName)
{
	if (m_pTextField)
	{
		m_pTextField->setSystemFontName(pFontName);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setFontName(pFontName);
	}
}

void TextInput::setFontSize(int fontSize)
{
	if (m_pTextField)
	{
		m_pTextField->setSystemFontSize(fontSize);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setFontSize(fontSize);
	}
}

void TextInput::setFont(const char* pFontName, int fontSize)
{
	if (m_pTextField)
	{
		m_pTextField->setSystemFontName(pFontName);
		m_pTextField->setSystemFontSize(fontSize);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setFontName(pFontName);
		m_pEditBox->setFontSize(fontSize);
	}
}


void TextInput::setPlaceholderFontName(const char* pFontName)
{
	if (m_pEditBox)
	{
		m_pEditBox->setPlaceholderFontName(pFontName);
	}
}

void TextInput::setPlaceholderFontSize(int fontSize)
{
	if (m_pEditBox)
	{
		m_pEditBox->setPlaceholderFontSize(fontSize);
	}
}
void TextInput::setPlaceholderFont(const char* pFontName, int fontSize)
{
	if (m_pEditBox)
	{
		m_pEditBox->setPlaceholderFontName(pFontName);
		m_pEditBox->setPlaceholderFontSize(fontSize);
	}
}

void TextInput::setMaxLength(int maxLength)
{
	if (m_pTextField)
	{
		m_nCharLimit = maxLength;
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setMaxLength(maxLength);
	}
}

void TextInput::setReturnType(cocos2d::ui::EditBox::KeyboardReturnType returnType)
{
	if (m_pEditBox)
	{
		m_pEditBox->setReturnType(returnType);
	}
}

void TextInput::setInputMode(cocos2d::ui::EditBox::InputMode inputMode)
{
	if (m_pEditBox)
	{
		m_pEditBox->setInputMode(inputMode);
	}
}

void TextInput::setInputFlag(cocos2d::ui::EditBox::InputFlag inputFlag)
{
	if (m_pTextField)
	{
		m_pTextField->setSecureTextEntry(true);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setInputFlag(inputFlag);
	}
}

void TextInput::setDelegate(void* pDelegate)
{
	//注意！！！！ 注册的不是传来的 pDelegate。
	if (m_pTextField)
	{
		m_pTextField->setDelegate(this);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setDelegate(this);
	}
}

void TextInput::setHorizontalAlignment(cocos2d::TextHAlignment alignment)
{
	if (m_pTextField)
	{
		m_pTextField->setHorizontalAlignment(alignment);
	}
}

void TextInput::setVerticalAlignment(cocos2d::TextVAlignment verticalAlignment)
{
	if (m_pTextField)
	{
		m_pTextField->setVerticalAlignment(verticalAlignment);
	}
}

void TextInput::setOnlyNum(bool m_isOnlyNum)
{
	this->m_isOnlyNum = m_isOnlyNum;

	if (m_pEditBox)
	{
		m_pEditBox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
	}
}

cocos2d::Color3B TextInput::getColorSpaceHolder()
{
	cocos2d::Color3B color;
	if (m_pTextField)
	{
		cocos2d::Color4B color4B = m_pTextField->getColorSpaceHolder();
		color = cocos2d::Color3B(color4B.r, color4B.g, color4B.b);
	}
	else
	{
		color = cocos2d::Color3B(255, 255, 255);
	}

	return color;
}

void TextInput::setColorSpaceHolder(const cocos2d::Color3B& color)
{
	if (m_pTextField)
	{
		m_pTextField->setColorSpaceHolder(color);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setPlaceholderFontColor(color);
	}
}

void TextInput::setLabelEx(bool isLabelEx)
{
	this->m_isLabelEx = isLabelEx;
}


cocos2d::Rect TextInput::getRect(cocos2d::Node * pNode)
{
	cocos2d::Size contentSize = pNode->getContentSize();
	cocos2d::Rect rect = cocos2d::Rect(0, 0, contentSize.width, contentSize.height);
	return cocos2d::RectApplyAffineTransform(rect, pNode->getNodeToWorldAffineTransform());
}

void TextInput::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	if (m_pEditBox)
	{
		if (m_isLabelEx)
		{
			m_pEditBox->setVisible(true);  //textField.hidden = YES
		}
	}

	cocos2d::log("editBox %p DidBegin !", editBox);  
}
void TextInput::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, cocos2d::ui::EditBoxDelegate::EditBoxEndAction action)
{
	cocos2d::log("editBox %p DidEnd !", editBox); 
}
void TextInput::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	cocos2d::log("editBox %p TextChanged, text: %s ", editBox, text.c_str());  
	if (m_pEditBox)
	{
		/*if (m_isLabelEx)
		{
			GameGobal::getInstance().getGameUILayer()->getChatPanel()->updateChatInput(m_pEditBox->getText());
		}*/
	}
}
void TextInput::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	cocos2d::log("editBox %p was returned !", editBox); 
}

void TextInput::setVisible(bool isVisible)
{
	if (m_pTextField)
	{
		m_pTextField->setVisible(isVisible);
	}
	else if (m_pEditBox)
	{
		m_pEditBox->setVisible(isVisible);
	}
}



