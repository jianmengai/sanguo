#pragma once
#include "cocos2d.h"

class TextureSprite : public cocos2d::Sprite
{
public:
	virtual bool init(int w, int h);
	static TextureSprite* create(int w, int h);
	//set with color
	void changeData(int x, int y, cocos2d::Color4B color);
	void clearWithColor(cocos2d::Color4B color);
	~TextureSprite();
private:
	void update(float dt);
private:
	cocos2d::Size m_layerSize;
	unsigned char *m_data;
};
