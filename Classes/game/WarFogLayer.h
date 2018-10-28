#pragma once

#include "cocos2d.h"
#include "TextureSprite.h"
class WarFogLayer :public cocos2d::Layer
{
public:
	static WarFogLayer* create(int w, int h);
	virtual bool init(int w, int h);
	void setTileSize(cocos2d::Size s);
	void inView(int x, int y);
	void outView(int x, int y);
	void setFogPosition(cocos2d::Vec2& pos);
	WarFogLayer();
	~WarFogLayer();
private:
	cocos2d::Size m_layerSize;
	TextureSprite* m_base;
	cocos2d::Color4B m_in;
	cocos2d::Color4B m_out;
	float m_scale;
};
