#pragma once
#include "cocos2d.h"
#include "ui/UILoadingBar.h"
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

	

	virtual void reduceHP(int amount);
	virtual void addHP(int amount);

	virtual float getAoeDamageRadius();

	//void setForceType(ForceType forceType) { m_forceType = forceType; }
	ForceType getForceType() { return m_forceType; }
	GameObjectType getGameObjectType() { return m_objectType; }
	GameObjectStatus getGameObjectStatus() { return m_objectStatus; }
	void isSelected(bool selected) { m_selected = selected; }
	bool isSelected() { return m_selected; }

	void setAttackTarget(GameObject* target);

	void update(float dt);

	virtual bool isReadyToRemove() = 0;

	virtual void setVisible(bool isVisible)
	{
		cocos2d::Sprite::setVisible(isVisible);
	}

	void showHpBar(bool show);

	bool canBeAttacked();

protected:
	GameObject();
	bool init() override;

	virtual void initHpBar() = 0;
	void showHpBar();
	void hideHpBar();

	void updateHp();

	virtual void onPrepareToRemove() = 0;
	

protected:
	int m_uniqId = 0;
	int m_enemyId = 0;     //自动警戒过程中触发的攻击目标
	int m_curHp = 0;
	int m_maxHp = 0;
	int m_maxDefence = 0;
	int m_curDefence = 0;
	GameObjectStatus m_objectStatus = GameObjectStatus::Invalid;
	//int m_type;
	int m_attackPower = 0;
	//int m_maxAttackPower = 0;
	float m_alertDistance = 1200; //警戒距离，距离内会被发现
	float m_attackDistance = 500; //攻击距离，距离内直接攻击
	BulletType m_bulletType = BulletType::Invalid;
	DamageType m_damageType = DamageType::Invalid;
	ForceType m_forceType = ForceType::Invalid;
	GameObjectType m_objectType = GameObjectType::Invalid;
	
	bool m_readyToRemove;
	bool m_selected = false;

	GameObject* m_attackTarget = nullptr;  //待攻击的目标，这里手工指定的或者预先选定的攻击目标

	cocos2d::ui::LoadingBar* m_hpBar = nullptr;

	bool m_showHpBar = true;
	bool m_canBeAttacked = true;
};