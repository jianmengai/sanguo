#include "GameDefine.h"
#include "GameObjectSelectBox.h"


void GameObjectSelectBox::setMouseDownStatus(bool isMouseDown)
{
	m_isMouseDown = isMouseDown;
}

void GameObjectSelectBox::setMouseDownPoint(const cocos2d::Vec2& point)
{
	m_mouseDownPoint = point;
}

void GameObjectSelectBox::syncCursorPoint(const cocos2d::Vec2& point)
{
	m_cursorPoint = point;
}

void GameObjectSelectBox::update(float delta)
{
	clear();

	if (m_isMouseDown)
	{
		m_leftBottomPoint.x = std::min(m_mouseDownPoint.x, m_cursorPoint.x);
		m_leftBottomPoint.y = std::min(m_mouseDownPoint.y, m_cursorPoint.y);

		m_boxSize.width = std::abs(m_mouseDownPoint.x - m_cursorPoint.x);
		m_boxSize.height = std::abs(m_mouseDownPoint.y - m_cursorPoint.y);

		m_rightTopPoint.x = m_leftBottomPoint.x + m_boxSize.width;
		m_rightTopPoint.y = m_leftBottomPoint.y + m_boxSize.height;

		drawSolidRect(m_leftBottomPoint, m_rightTopPoint, m_color);
		drawSolidRect(m_leftBottomPoint, m_rightTopPoint, m_color);

		m_rect.setRect(m_leftBottomPoint.x, m_leftBottomPoint.y, m_boxSize.width, m_boxSize.height);
	}
}

bool GameObjectSelectBox::init()
{
	if (!DrawNode::init())
	{
		return false;
	}

	m_color = cocos2d::Color4F(0.0f, 0.0f, 1.0f, 0.2f);

	setPositionZ(MAX_GAME_OBJECT_COUNT);

	scheduleUpdate();

	return true;
}

cocos2d::Rect& GameObjectSelectBox::getRect()
{
	return m_rect;
}
