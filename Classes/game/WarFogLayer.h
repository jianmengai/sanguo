#pragma once

#include "cocos2d.h"
#include "TextureSprite.h"
class WarFogLayer :public cocos2d::Layer
{
public:
	static WarFogLayer* getInstance();
	virtual bool init(cocos2d::Layer* parentLayer, int w, int h);
	void inView(int x, int y);
	void outView(int x, int y);
	void setPosition(cocos2d::Vec2& pos, bool isDelta = true);
	WarFogLayer();
	~WarFogLayer();
private:
	cocos2d::Vec2 getFogPosition();
	void setFogPosition(cocos2d::Vec2& pos);
private:
	cocos2d::Size m_layerSize;
	cocos2d::Size m_clientWinSize;
	cocos2d::Size m_mapContentSize;
	TextureSprite* m_base;
	cocos2d::Color4B m_in;
	cocos2d::Color4B m_out;
	float m_scale;
	float m_scaleY;
};
