#include "Menu.h"
#include "SimpleButton.h"

constexpr auto kDefaultPadding = 5;


Menu* Menu::create()
{
	return Menu::create(nullptr, nullptr);
}

Menu* Menu::create(SimpleButton* item, ...)
{
	va_list args;
	va_start(args, item);

	Menu *pRet = Menu::createWithItems(item, args);

	va_end(args);
	return pRet;
}

Menu* Menu::createWithArray(cocos2d::Vector<SimpleButton*>& arrayOfItems)
{
	Menu *pRet = new Menu();
	if (pRet && pRet->initWithArray(arrayOfItems))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

Menu* Menu::createWithItems(SimpleButton* item, va_list args)
{
	if (item)
	{
		cocos2d::Vector<SimpleButton*> array;
		array.pushBack(item);
		SimpleButton *i = va_arg(args, SimpleButton*);
		while (i)
		{
			array.pushBack(i);
			i = va_arg(args, SimpleButton*);
		}
		return Menu::createWithArray(array);
	}
	return nullptr;
}

Menu* Menu::createWithItem(SimpleButton* item)
{
	return Menu::create(item, NULL);
}

bool Menu::init()
{
	cocos2d::Vector<SimpleButton*> emptyArray;
	return initWithArray(emptyArray);
}

bool Menu::initWithArray(cocos2d::Vector<SimpleButton*>& arrayOfItems)
{
	m_bEnabled = true;
	// menu in the center of the screen
	cocos2d::Size s = cocos2d::Director::getInstance()->getWinSize();

	this->ignoreAnchorPointForPosition(true);
	setAnchorPoint(cocos2d::Point(0.5f, 0.5f));
	this->setContentSize(s);

	setPosition(cocos2d::Point(s.width / 2, s.height / 2));

	m_horizontallyAlignType = ALIGN_ITEM_HORIZONTALLY_CENTER;

	
	int z = 0;
	for (auto& i : arrayOfItems)
	{
		this->addChild(i, z++);
	}
	

	//    [self alignItemsVertically];
	m_pSelectedItem = NULL;
	// enable cascade color and opacity on menus
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);

	return true;
}

/*
* override add:
*/
void Menu::addChild(cocos2d::Node * child)
{
	cocos2d::Node::addChild(child);
}

void Menu::addChild(cocos2d::Node * child, int zOrder)
{
	cocos2d::Node::addChild(child, zOrder);
}

void Menu::addChild(cocos2d::Node * child, int zOrder, int tag)
{
	CCAssert(dynamic_cast<SimpleButton*>(child) != NULL, "Menu only supports MenuItem objects as children");
	cocos2d::Node::addChild(child, zOrder, tag);
}

void Menu::onExit()
{
	if (m_pSelectedItem)
	{
		m_pSelectedItem->selected(false);
		m_pSelectedItem = NULL;
	}
	cocos2d::Node::onExit();
}

void Menu::removeChild(cocos2d::Node* child, bool cleanup)
{
	SimpleButton *pMenuItem = dynamic_cast<SimpleButton*>(child);
	CCAssert(pMenuItem != NULL, "Menu only supports MenuItem objects as children");

	if (m_pSelectedItem == pMenuItem)
	{
		m_pSelectedItem = NULL;
	}

	cocos2d::Node::removeChild(child, cleanup);
}
bool Menu::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	CC_UNUSED_PARAM(event);
	if (!m_bEnabled || !isShow())
	{
		return false;
	}

	m_pSelectedItem = this->itemForTouch(touch);
	if (m_pSelectedItem)
	{
		m_pSelectedItem->selected(true);
		return true;
	}
	return false;
}

bool Menu::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event* event)
{
	bool bRet = false;
	CC_UNUSED_PARAM(touch);
	CC_UNUSED_PARAM(event);
	SimpleButton* currentItem = itemForTouch(touch);
	if (currentItem)
	{
		if (m_pSelectedItem)
		{
			m_pSelectedItem->selected(false);
			if (m_pSelectedItem == currentItem)
			{
				m_pSelectedItem->activate();
			}
			m_pSelectedItem = NULL;
		}
		return true;
	}
	if (m_pSelectedItem)
	{
		m_pSelectedItem->selected(false);
		m_pSelectedItem = NULL;
	}
	return false;
}

void Menu::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event* event)
{
	CC_UNUSED_PARAM(touch);
	CC_UNUSED_PARAM(event);
	if (m_pSelectedItem)
	{
		m_pSelectedItem->selected(false);
		m_pSelectedItem = NULL;
	}
}

void Menu::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	CC_UNUSED_PARAM(event);
}

//Menu - Alignment
void Menu::alignItemsVertically()
{
	this->alignItemsVerticallyWithPadding(kDefaultPadding);
}

void Menu::alignItemsVerticallyWithPadding(float padding)
{
	float height = -padding;
	if (_children.size() > 0)
	{
		for (auto& node : _children)
		{
			if (node)
			{
				height += node->getContentSize().height * node->getScaleY() + padding;
			}
		}
	}

	float y = height / 2.0f;
	if (_children.size() > 0)
	{
		for (auto& node : _children)
		{
			if (node)
			{
				node->setPosition(cocos2d::Point(0, y - node->getContentSize().height * node->getScaleY() / 2.0f));
				y -= node->getContentSize().height * node->getScaleY() + padding;
			}
		}
	}
}

void Menu::alignItemsHorizontally(void)
{
	this->alignItemsHorizontallyWithPadding(kDefaultPadding);
}

void Menu::alignItemsHorizontallyWithPadding(float padding)
{
	float width = 0;
	switch (m_horizontallyAlignType)
	{
	case ALIGN_ITEM_HORIZONTALLY_CENTER:
	{
		width = -padding;
		if (_children.size() > 0)
		{
			for (auto& node : _children)
			{
				if (node && node->isVisible())
				{
					width += node->getContentSize().width * node->getScaleX() + padding;
				}
			}
		}
		float x = -width / 2.0f;
		if (_children.size() > 0)
		{
			for (auto& node : _children)
			{
				if (node && node->isVisible())
				{
					node->setPosition(cocos2d::Point(x + node->getContentSize().width * node->getScaleX() / 2.0f, node->getContentSize().height * node->getScaleY() / 2.0f));
					x += node->getContentSize().width * node->getScaleX() + padding;
				}
			}
		}
	}
	break;
	case ALIGN_ITEM_HORIZONTALLY_LEFT:
	{
		if (_children.size() > 0)
		{
			for (auto& node : _children)
			{
				if (node && node->isVisible())
				{
					node->setPosition(cocos2d::Point(width + node->getContentSize().width * node->getScaleX() / 2.0f, node->getContentSize().height * node->getScaleY() / 2.0f));
					width += node->getContentSize().width * node->getScaleX() + padding;
				}
			}
		}
	}
	break;
	case ALIGN_ITEM_HORIZONTALLY_RIGHT:
	{
		if (_children.size() > 0)
		{
			for (auto& node : _children)
			{
				if (node && node->isVisible())
				{
					width += node->getContentSize().width * node->getScaleX() + padding;
				}
			}
		}
		float x = -width;
		if (_children.size() > 0)
		{
			for (auto& node : _children)
			{
				if (node && node->isVisible())
				{
					node->setPosition(cocos2d::Point(x + node->getContentSize().width * node->getScaleX() / 2.0f, node->getContentSize().height * node->getScaleY() / 2.0f));
					x += node->getContentSize().width * node->getScaleX() + padding;
				}
			}
		}
	}
	break;
	}
}

SimpleButton* Menu::itemForTouch(cocos2d::Touch *touch)
{
	cocos2d::Point touchLocation = touch->getLocation();
	if (_children.size() > 0)
	{
		for (auto node : _children)
		{
			SimpleButton* pChild = dynamic_cast<SimpleButton*>(node);
			if (pChild && pChild->isVisible() && pChild->isEnabled())
			{
				cocos2d::Point local = pChild->convertToNodeSpace(touchLocation);
				cocos2d::Rect r = pChild->rect();
				r.origin = cocos2d::Point(0, 0);
				if (r.containsPoint(local))
				{
					return pChild;
				}
			}
		}
	}

	return NULL;
}
