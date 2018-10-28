#include "WarFogLayer.h"
#include "MapManager.h"
WarFogLayer::WarFogLayer()
{
}
WarFogLayer::~WarFogLayer()
{
}

WarFogLayer* WarFogLayer::create(int w, int h)
{
	auto warFogLayer = new (std::nothrow)WarFogLayer;
	if (warFogLayer && warFogLayer->init(w, h))
	{
		warFogLayer->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(warFogLayer);
	}

	return warFogLayer;
}


bool WarFogLayer::init(int w, int h) 
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	m_layerSize = cocos2d::Size(w, h);
	m_base = TextureSprite::create(w, h);
	addChild(m_base);
	m_base->setRotation(45);
	m_out = cocos2d::Color4B::BLUE;
	m_in = m_out;
	m_out.a = 120;
	m_in.a = 0;
	m_scale = MapManager::getInstance()->getMapScale();
	auto mapContentSize = MapManager::getInstance()->getContentSize() * m_scale;
	//m_base->setPosition(cocos2d::Vec2(0, mapContentSize.height / 2.0));
	cocos2d::log("anchor point:%0.1f, %0.1f, position:%0.1f, %0.1f", m_base->getAnchorPoint().x, m_base->getAnchorPoint().y, 
		m_base->getPosition().x, m_base->getPosition().y);
	cocos2d::log("war fog size:%0.1f,%0.1f", m_base->getContentSize().width, m_base->getContentSize().height);
	return true;
}
void WarFogLayer::setTileSize(cocos2d::Size s) 
{
	auto scale = m_scale * s.width / sqrt(2);
	m_base->setScale(scale);
	setScaleY(s.height / s.width * scale);
}

void WarFogLayer::setFogPosition(cocos2d::Vec2& pos)
{
	cocos2d::Vec2 newPosition = pos * m_scale;
	
		cocos2d::Vec2 curPos = m_base->getPosition();
		cocos2d::log("old warfog postion, x:%0.1f, y:%0.1f", curPos.x, curPos.y);
		pos.negate();

		newPosition = curPos - newPosition;

	m_base->setPosition(newPosition);
}

void WarFogLayer::inView(int x, int y) 
{
	if (x >= 0 && y >= 0 && x < m_layerSize.width&&y < m_layerSize.height)
	{
		cocos2d::log("inView:%d, %d",x, y);
		m_base->changeData(x, y, m_in);
	}

}
void WarFogLayer::outView(int x, int y) 
{
	if (x >= 0 && y >= 0 && x < m_layerSize.width&&y < m_layerSize.height)
	{
		m_base->changeData(x, y, m_out);
	}
}
