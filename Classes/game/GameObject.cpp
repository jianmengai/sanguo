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

void GameObject::update(float dt)
{
	updateHp();
	/*if (!isSelected())
	{
		hideHpBar();
	}*/
}

void GameObject::reduceHP(int amount)
{
	showHpBar();
	m_curHp = (m_curHp >= amount) ? (m_curHp - amount) : 0;
	cocos2d::log("id:%d, hp:%d", m_uniqId, m_curHp);
	if (m_curHp <= 0)
	{
		hideHpBar();
		onPrepareToRemove();
	}
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

void GameObject::showHpBar()
{
	auto hpBarBackground = m_hpBar->getParent();
	hpBarBackground->setVisible(true);
}

void GameObject::hideHpBar()
{
	auto hpBarBackground = m_hpBar->getParent();
	hpBarBackground->setVisible(false);
}

void GameObject::updateHp()
{
	float hpPercent = (float)m_curHp / (float)m_maxHp;
	m_hpBar->setPercent(hpPercent * 100.0f);
}
