#pragma once
#include "cocos2d.h"
#include "GameDefine.h"

class GameObject : public cocos2d::Sprite
{
public:
	~GameObject();
	virtual int getId() { return m_uniqId; }
	virtual void setEnemyId(int enemyId) { m_enemyId = enemyId; }
	virtual bool itemIsTouched(cocos2d::Point& touchLocation) { return true; }
	virtual DamageType getDamageType() { return m_damageType; }
	virtual int getAttackPower() { return m_attackPower; }

	virtual bool isReadyToRemove() { return m_readyToRemove; }

	virtual void reduceHP(int amount);
	virtual void addHP(int amount);

	virtual float getAoeDamageRadius();

	void setForceType(ForceType forceType) { m_forceType = forceType; }
	ForceType getForceType() { return m_forceType; }
	GameObjectType getGameObjectType() { return m_objectType; }
	GameObjectStatus getGameObjectStatus() { return m_objectStatus; }
protected:
	GameObject();
	bool init() override;
protected:
	int m_uniqId;
	int m_enemyId;
	int m_curHp;
	int m_maxHp;
	int m_maxDefence;
	int m_curDefence;
	GameObjectStatus m_objectStatus = GameObjectStatus::Invalid;
	int m_type;
	int m_attackPower;
	int m_maxAttackPower;
	BulletType m_bulletType = BulletType::Invalid;
	DamageType m_damageType = DamageType::Invalid;
	ForceType m_forceType = ForceType::Invalid;
	GameObjectType m_objectType = GameObjectType::Invalid;
	
	bool m_readyToRemove;
};