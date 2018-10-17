#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

bool GameObject::init()
{
	if (!cocos2d::Sprite::init())
	{
		return false;
	}

	return true;
}

void GameObject::reduceHP(int amount)
{
	m_curHp = (m_curHp >= amount) ? (m_curHp - amount) : 0;
}

void GameObject::addHP(int amount)
{
	int tempHp = m_curHp + amount;
	m_curHp = (tempHp <= m_maxHp) ? tempHp : m_maxHp;
}

float GameObject::getAoeDamageRadius()
{
	return 0.0f;
}
