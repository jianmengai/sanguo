#include "WarFogLayer.h"

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
	//m_base->setRotation(45);
	m_out = cocos2d::Color4B::BLACK;
	m_in = m_out;
	m_out.a = 90;
	m_in.a = 0;
	//setScaleY(0.5f);
	return true;
}
void WarFogLayer::setTileSize(cocos2d::Size s) 
{
	m_base->setScale(s.height*sqrt(2));
}
void WarFogLayer::inView(int x, int y) 
{
	if (x >= 0 && y >= 0 && x < m_layerSize.width&&y < m_layerSize.height)
	{
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
