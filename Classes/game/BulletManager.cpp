#include "BulletManager.h"
#include "GameObjectManager.h"
#include "GameUtils.h"
#include "GameConfig.h"
#include "SpecialEffectManager.h"

BulletManager* BulletManager::getInstance()
{
	static BulletManager bulletManagerInstance;
	return &bulletManagerInstance;
}

cocos2d::Sprite* BulletManager::createBullet(BulletType bulletType, int attackerId, int attackTargetId)
{
	cocos2d::Sprite* bullet = nullptr;
	GameObject* attacker = GameObjectManager::getInstance()->getGameObjectById(attackerId);
	GameObject* attackTarget = GameObjectManager::getInstance()->getGameObjectById(attackTargetId);
	if (attacker == nullptr || attackTarget == nullptr)
	{
		return nullptr;
	}
	if (attacker && attackTarget && bulletType != BulletType::Invalid)
	{
		auto bulletConf = GameConfig::getInstance()->getBulletConf(bulletType);
		bullet = cocos2d::Sprite::create(bulletConf->fileName);

		auto attackerPosition = attacker->getPosition();
		auto targetPosition = attackTarget->getPosition();

		/*if (bulletType == BulletType::Bomb)
		{
			attackerPosition.y = attackerPosition.y - attacker->getContentSize().height / 2.0f;
			targetPosition.x = attackerPosition.x;
		}*/

		bullet->setPosition(attackerPosition);

		auto rotation = GameUtils::computeRotatedDegree(attackerPosition, targetPosition);
		bullet->setRotation(rotation);

		auto distance = GameUtils::computeDistanceBetween(attackerPosition, targetPosition);

		auto duration = distance / bulletConf->speed;
		auto moveTo = cocos2d::MoveTo::create(duration, targetPosition);

		cocos2d::CallFunc* onMoveEnd = nullptr;
		if (attacker->getDamageType() == DamageType::AreaOfEffect)
		{
			onMoveEnd = cocos2d::CallFunc::create(CC_CALLBACK_0(BulletManager::onAOEDamageBulletMoveEnd, this, bullet, bulletType, attacker, targetPosition));
		}
		else
		{
			onMoveEnd = cocos2d::CallFunc::create(
				CC_CALLBACK_0(BulletManager::onNormalDamageBulletMoveEnd,
					this,
					bullet,
					bulletType,
					attackTarget->getId(),
					attacker->getAttackPower()));
		}

		auto sequenceAction = cocos2d::Sequence::create(moveTo, onMoveEnd, nullptr);
		bullet->runAction(sequenceAction);
	}

	return bullet;
}

void BulletManager::onNormalDamageBulletMoveEnd(cocos2d::Sprite* bullet, BulletType bulletType, int attackTargetID, int damageAmount)
{
	auto attackTarget = GameObjectManager::getInstance()->getGameObjectById(attackTargetID);
	if (attackTarget && !attackTarget->isReadyToRemove())
	{
		attackTarget->reduceHP(damageAmount);
		onCreateSpecialEffect(bulletType, attackTarget->getPosition());
	}

	bullet->retain();
	bullet->removeFromParent();
	bullet->autorelease();
}

void BulletManager::onAOEDamageBulletMoveEnd(cocos2d::Sprite* bullet, BulletType bulletType, GameObject* attacker, const cocos2d::Vec2& endPosition)
{
	/*auto attackerForceType = attacker->getForceType();
	auto attackerAoeDamageRadius = attacker->getAoeDamageRadius();
	auto attackerDamagePower = attacker->getAttackPower();

	auto gameObjectsMap = GameObjectManager::getInstance()->getGameObjectMap();

	vector<int> attackedTargetUniqueIDList;
	for (auto& gameObjectIter : gameObjectsMap)
	{
		auto gameObject = gameObjectIter.second;
		if (gameObject->isReadyToRemove() ||
			gameObject->getForceType() == attackerForceType ||
			gameObject->getGameObjectType() == GameObjectType::DefenceInBuildingNpc)
		{
			continue;
		}

		auto gameObjectPosition = gameObject->getPosition();
		auto distance = GameUtils::computeDistanceBetween(endPosition, gameObjectPosition);
		if (distance <= attackerAoeDamageRadius)
		{
			attackedTargetUniqueIDList.push_back(gameObject->getUniqueID());
		}
	}

	for (auto uniqueID : attackedTargetUniqueIDList)
	{
		auto gameObjectIter = gameObjectsMap.find(uniqueID);
		gameObjectIter->second->costHP(attackerDamagePower);
	}

	onCreateSpecialEffect(bulletType, endPosition);

	bullet->retain();
	bullet->removeFromParent();
	bullet->autorelease();*/
}

void BulletManager::onCreateSpecialEffect(BulletType bulletType, const cocos2d::Vec2& inMapPosition)
{
	auto bulletConf = GameConfig::getInstance()->getBulletConf(bulletType);
	SpecialEffectManager::getInstance()->createSpecialEffect(bulletConf->specialEffect, inMapPosition, false);
}