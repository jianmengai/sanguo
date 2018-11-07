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
	void isSelected(bool selected) { m_selected = selected; }
	bool isSelected() { return m_selected; }

	void setAttackTarget(GameObject* target);

protected:
	GameObject();
	bool init() override;
protected:
	int m_uniqId;
	int m_enemyId;     //自动警戒过程中触发的攻击目标
	int m_curHp;
	int m_maxHp;
	int m_maxDefence;
	int m_curDefence;
	GameObjectStatus m_objectStatus = GameObjectStatus::Invalid;
	//int m_type;
	int m_attackPower;
	int m_maxAttackPower;
	float m_alertDistance = 900; //警戒距离，距离内会被发现
	float m_attackDistance = 300; //攻击距离，距离内直接攻击
	BulletType m_bulletType = BulletType::Invalid;
	DamageType m_damageType = DamageType::Invalid;
	ForceType m_forceType = ForceType::Invalid;
	GameObjectType m_objectType = GameObjectType::Invalid;
	
	bool m_readyToRemove;
	bool m_selected = false;

	GameObject* m_attackTarget = nullptr;  //待攻击的目标，这里手工指定的或者预先选定的攻击目标
};