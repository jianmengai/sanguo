#include "TextureSprite.h"

TextureSprite* TextureSprite::create(int w, int h)
{
	auto textureSprite = new (std::nothrow)TextureSprite;
	if (textureSprite && textureSprite->init(w, h))
	{
		textureSprite->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(textureSprite);
	}

	return textureSprite;
}

bool TextureSprite::init(int w, int h)
{
	if (!cocos2d::Sprite::init())
	{
		return false;
	}
	auto dataLen = w * h * 4;
	m_data = new unsigned char[dataLen];
	auto allcolor = cocos2d::Color4B::BLACK;
	//allcolor.a = 150;
	m_layerSize = cocos2d::Size(w, h);
	clearWithColor(allcolor);
	auto texture = new (std::nothrow)cocos2d::Texture2D();
	if (texture)
	{
		texture->initWithData(m_data, dataLen, cocos2d::Texture2D::PixelFormat::RGBA8888, w, h, cocos2d::Size((float)w, (float)h));
	}
	initWithTexture(texture);
	texture->release();
	setAnchorPoint(cocos2d::Vec2(m_layerSize.height / m_layerSize.width - 0.5f, -0.5f*(m_layerSize.width / m_layerSize.height)));
	scheduleUpdate();
	return true;
}
void TextureSprite::update(float dt) 
{
	_texture->updateWithData(m_data, 0, 0, getContentSize().width, getContentSize().height);
}
void TextureSprite::clearWithColor(cocos2d::Color4B color) 
{
	int w = 4 * m_layerSize.width;
	for (int i = 0; i < m_layerSize.height; i++)
	{
		for (int j = 0; j < m_layerSize.width; j++)
		{
			m_data[i * w + j * 4 + 0] = color.r;
			m_data[i * w + j * 4 + 1] = color.g;
			m_data[i * w + j * 4 + 2] = color.b;
			m_data[i * w + j * 4 + 3] = color.a;
		}
	}
}
void TextureSprite::changeData(int x, int y, cocos2d::Color4B color) 
{
	int w = 4 * m_layerSize.width;
	m_data[y * w + x * 4 + 0] = color.r;
	m_data[y * w + x * 4 + 1] = color.g;
	m_data[y * w + x * 4 + 2] = color.b;
	m_data[y * w + x * 4 + 3] = color.a;
}
TextureSprite::~TextureSprite() 
{
	delete m_data;
}
