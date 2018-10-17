#pragma once

#include "cocos2d.h"

class GameObjectSelectBox : public cocos2d::DrawNode
{
public:
	CREATE_FUNC(GameObjectSelectBox);

	void setMouseDownStatus(bool isMouseDown);
	void setMouseDownPoint(const cocos2d::Vec2& point);

	void syncCursorPoint(const cocos2d::Vec2& point);
	void update(float delta) override;
	cocos2d::Rect& getRect();
private:
	bool init() override;

	bool m_isMouseDown = false;
	cocos2d::Vec2 m_mouseDownPoint;
	cocos2d::Vec2 m_cursorPoint;

	cocos2d::Color4F m_color;
	cocos2d::Size m_boxSize;
	cocos2d::Vec2 m_leftBottomPoint;
	cocos2d::Vec2 m_rightTopPoint;
	cocos2d::Rect m_rect;
};