#include "WarFogLayer.h"
#include "MapManager.h"
WarFogLayer::WarFogLayer()
{
}
WarFogLayer::~WarFogLayer()
{
}

WarFogLayer* WarFogLayer::getInstance()
{
	static WarFogLayer warFogLayerInstance;
	return &warFogLayerInstance;
}

bool WarFogLayer::init(cocos2d::Layer* parentLayer, int w, int h) 
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	m_layerSize = cocos2d::Size(w, h);
	m_base = TextureSprite::create(w, h);
	addChild(m_base);
	
	m_out = cocos2d::Color4B::BLUE;
	m_in = m_out;
	m_out.a = 120;
	m_in.a = 0;
	m_scale = MapManager::getInstance()->getMapScale();
	m_mapContentSize = MapManager::getInstance()->getContentSize() * m_scale;
	auto tileSize = MapManager::getInstance()->getTileSize();
	auto scale = m_scale * tileSize.width / sqrt(2);
	m_base->setScale(scale);
	m_base->setRotation(45);

	m_scaleY = tileSize.height / tileSize.width;

	//setFogPosition(cocos2d::Vec2(0, m_mapContentSize.height / 2.0));

	setAnchorPoint(cocos2d::Vec2(0.0f, 1.0f));
	setScaleY(tileSize.height / tileSize.width);
	m_clientWinSize = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::log("anchor point:%0.1f, %0.1f, position:%0.1f, %0.1f, layer:%0.1f, %0.1f, anchor:%0.1f, %0.1f", m_base->getAnchorPoint().x, m_base->getAnchorPoint().y, 
		m_base->getPosition().x, m_base->getPosition().y, this->getPosition().x, this->getPosition().y, this->getAnchorPoint().x, this->getAnchorPoint().y);
	cocos2d::log("war fog size:%0.1f,%0.1f, layer size:%0.1f, %0.1f", m_base->getContentSize().width, m_base->getContentSize().height, this->getContentSize().width, this->getContentSize().height);
	
	parentLayer->addChild(this);
	
	return true;
}

void WarFogLayer::setFogPosition(cocos2d::Vec2& pos)
{
	pos.y /= m_scaleY;
	//cocos2d::log("----set war fog, x:%0.1f, y:%0.1f", pos.x, pos.y);
	m_base->setPosition(pos);
}

cocos2d::Vec2 WarFogLayer::getFogPosition()
{
	auto pos = m_base->getPosition();
	pos.y *= m_scaleY;
	return pos;
}

void WarFogLayer::setPosition(cocos2d::Vec2& pos, bool isDelta)
{
	cocos2d::Vec2 newPosition = pos;
	cocos2d::Vec2 curPos = getFogPosition();
	auto minY = m_clientWinSize.height - m_mapContentSize.height / 2.0;
	auto maxY = m_mapContentSize.height / 2.0;
	auto minX = m_clientWinSize.width - m_mapContentSize.width;
	if (isDelta)
	{
		newPosition = curPos + newPosition;
	}
	if (newPosition.y > maxY)
	{
		newPosition.y = maxY;
	}
	if (newPosition.y < minY)
	{
		newPosition.y = minY;
	}
	if (newPosition.x > 0)
	{
		newPosition.x = 0;
	}
	if (newPosition.x < minX)
	{
		newPosition.x = minX;
	}
	setFogPosition(newPosition);
	cocos2d::log("new warfog positin, x:%0.1f, y:%0.1f, delta y:%0.1f, scale:%0.1f", newPosition.x, newPosition.y, pos.y, m_scale);
}

void WarFogLayer::inView(int x, int y) 
{
	//ÖÜ±ß¾Å¹¬¸ñ
	for (int i = x - 1; i <= x + 1; ++i)
	{
		for (int j = y - 1; j <= y + 1; ++j)
		{
			if (i >= 0 && j >= 0 && i < m_layerSize.width && j < m_layerSize.height)
			{
				cocos2d::log("inView:%d, %d", i, j);
				m_base->changeData(i, j, m_in);
			}
		}
	}
}
void WarFogLayer::outView(int x, int y) 
{
	if (x >= 0 && y >= 0 && x < m_layerSize.width&&y < m_layerSize.height)
	{
		m_base->changeData(x, y, m_out);
	}
}
