#include "BaseNode.h"


NodeRGBA::NodeRGBA()
	: _displayedOpacity(255)
	, _realOpacity(255)
	, _displayedColor(cocos2d::Color3B(255, 255, 255))
	, _realColor(cocos2d::Color3B(255, 255, 255))
	, _cascadeOpacityEnabled(false)
	, _cascadeColorEnabled(false)
{}

NodeRGBA::~NodeRGBA() {}

bool NodeRGBA::init()
{
	_displayedOpacity = _realOpacity = 255;
	_displayedColor = _realColor = cocos2d::Color3B(255, 255, 255);
	setCascadeOpacityEnabled(false);
	setCascadeColorEnabled(false);

	return true;
}

GLubyte NodeRGBA::getOpacity() const 
{
	return _realOpacity;
}

GLubyte NodeRGBA::getDisplayedOpacity() const
{
	return _displayedOpacity;
}

void NodeRGBA::setOpacity(GLubyte opacity)
{
	_displayedOpacity = _realOpacity = opacity;

	if (_cascadeOpacityEnabled)
	{
		GLubyte parentOpacity = 255;
		cocos2d::RGBAProtocol *parent = dynamic_cast<cocos2d::RGBAProtocol*>(_parent);
		if (parent && parent->isCascadeOpacityEnabled())
		{
			parentOpacity = parent->getDisplayedOpacity();
		}
		updateDisplayedOpacity(parentOpacity);
	}
}

const cocos2d::Color3B& NodeRGBA::getColor() const
{
	return _realColor;
}

const cocos2d::Color3B& NodeRGBA::getDisplayedColor() const
{
	return _displayedColor;
}

void NodeRGBA::setColor(const cocos2d::Color3B& color)
{
	_displayedColor = _realColor = color;

	if (_cascadeColorEnabled)
	{
		cocos2d::Color3B parentColor = cocos2d::Color3B(255, 255, 255);;
		cocos2d::RGBAProtocol* parent = dynamic_cast<cocos2d::RGBAProtocol*>(_parent);
		if (parent && parent->isCascadeColorEnabled())
		{
			parentColor = parent->getDisplayedColor();
		}

		updateDisplayedColor(parentColor);
	}
}

void NodeRGBA::updateDisplayedOpacity(GLubyte parentOpacity)
{
	_displayedOpacity = _realOpacity * parentOpacity / 255.0;

	if (_cascadeOpacityEnabled)
	{
		cocos2d::Object *obj = nullptr;
		for (auto& obj : _children)
		{
			cocos2d::RGBAProtocol *item = dynamic_cast<cocos2d::RGBAProtocol*>(obj);
			if (item)
			{
				item->updateDisplayedOpacity(_displayedOpacity);
			}
		}
	}
}

void NodeRGBA::updateDisplayedColor(const cocos2d::Color3B& parentColor)
{
	_displayedColor.r = _realColor.r * parentColor.r / 255.0;
	_displayedColor.g = _realColor.g * parentColor.g / 255.0;
	_displayedColor.b = _realColor.b * parentColor.b / 255.0;

	if (_cascadeColorEnabled)
	{
		cocos2d::Object *obj = nullptr;
		for (auto& obj : _children)
		{
			cocos2d::RGBAProtocol *item = dynamic_cast<cocos2d::RGBAProtocol*>(obj);
			if (item)
			{
				item->updateDisplayedColor(_displayedColor);
			}
		}
	}
}

bool NodeRGBA::isCascadeOpacityEnabled() const
{
	return _cascadeOpacityEnabled;
}

void NodeRGBA::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
	_cascadeOpacityEnabled = cascadeOpacityEnabled;
}

bool NodeRGBA::isCascadeColorEnabled() const
{
	return _cascadeColorEnabled;
}

void NodeRGBA::setCascadeColorEnabled(bool cascadeColorEnabled)
{
	_cascadeColorEnabled = cascadeColorEnabled;
}

BaseNode::BaseNode(void)
{
	m_pListener = nullptr;
	m_pfnSelector = nullptr;
}

BaseNode::~BaseNode(void)
{

}

void BaseNode::show(bool isShow)
{
	setVisible(isShow);
}

bool BaseNode::isShow()
{
	if (isVisible())
	{
		return hasVisibleParents();
	}
	return false;
}

bool BaseNode::onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event *pEvent)
{
	return false;
}

void BaseNode::onTouchMoved(cocos2d::Touch*pTouch, cocos2d::Event*pEvent)
{

}

bool BaseNode::onTouchEnded(cocos2d::Touch*pTouch, cocos2d::Event*pEvent)
{
	return false;
}

void BaseNode::onTouchCancelled(cocos2d::Touch*pTouch, cocos2d::Event*pEvent)
{

}

bool BaseNode::hasVisibleParents()
{
	cocos2d::Node* pParent = this->getParent();
	for (cocos2d::Node *c = pParent; c != nullptr; c = c->getParent())
	{
		if (!c->isVisible())
		{
			return false;
		}
	}
	return true;
}

cocos2d::Rect BaseNode::rect()
{
	return cocos2d::Rect(_position.x - _contentSize.width * _anchorPoint.x,
		_position.y - _contentSize.height * _anchorPoint.y,
		_contentSize.width, _contentSize.height);
}

void BaseNode::setTarget(cocos2d::Object *rec, cocos2d::SEL_CallFuncO selector)
{
	m_pListener = rec;
	m_pfnSelector = selector;
}
