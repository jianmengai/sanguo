#include "Soldier.h"
#include "GameObject.h"
#include "GameConfig.h"
#include "BulletManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "MapManager.h"
#include "GameUtils.h"
#include "AutoFindPath.h"
#include "building.h"

Soldier* Soldier::create(ForceType forceType, SoldierType type, const cocos2d::Vec2& position, FaceDirection direction)
{
	auto soldier = new (std::nothrow)Soldier;
	if (soldier && soldier->init(forceType, type, position, direction))
	{
		soldier->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(soldier);
	}

	return soldier;
}

bool Soldier::init(ForceType forceType, SoldierType type, const cocos2d::Vec2& position, FaceDirection direction)
{
	if (!GameObject::init())
	{
		return false;
	}

	m_forceType = forceType;
	m_objectType = GameObjectType::Soldier;
	m_faceDirection = direction;

	if (!initAnimate(type))
	{
		return false;
	}
	
	if (!initData(type))
	{
		return false;
	}
	
	this->setScale(1.5);
	this->setPosition(position);
	
	initHpBar();
	
	m_uniqId = GameUtils::getLastestUniqId();
	
	scheduleUpdate();

	return true;
}

void Soldier::clear()
{
	for (auto& moveAnimate : m_moveAnimateMap)
	{
		CC_SAFE_RELEASE_NULL(moveAnimate.second);
	}
	m_moveAnimateMap.clear();

	if (m_dieAnimate)
	{
		CC_SAFE_RELEASE_NULL(m_dieAnimate);
	}

	for (auto& standAnimate : m_standAnimateMap)
	{
		CC_SAFE_RELEASE_NULL(standAnimate.second);
	}
	m_standAnimateMap.clear();
}

bool Soldier::initAnimate(SoldierType type)
{
	auto soldierConf = GameConfig::getInstance()->getSoldierConf(m_forceType, type);
	if (soldierConf == nullptr)
	{
		return false;
	}

	m_moveAnimateMap[FaceDirection::FaceToEast] = createAnimateWithPlist(soldierConf->moveToEastAnimationPList, soldierConf->moveAnimateDelayPerUnit, GameObjectStatus::Move);
	m_moveAnimateMap[FaceDirection::FaceToNorthEast] = createAnimateWithPlist(soldierConf->moveToNorthEastAnimationPList, soldierConf->moveAnimateDelayPerUnit, GameObjectStatus::Move);
	m_moveAnimateMap[FaceDirection::FaceToNorthWest] = createAnimateWithPlist(soldierConf->moveToNorthWestAnimationPList, soldierConf->moveAnimateDelayPerUnit, GameObjectStatus::Move);
	m_moveAnimateMap[FaceDirection::FaceToSouthEast] = createAnimateWithPlist(soldierConf->moveToSouthEastAnimationPList, soldierConf->moveAnimateDelayPerUnit, GameObjectStatus::Move);
	m_moveAnimateMap[FaceDirection::FaceToSouthWest] = createAnimateWithPlist(soldierConf->moveToSouthWestAnimationPList, soldierConf->moveAnimateDelayPerUnit, GameObjectStatus::Move);
	m_moveAnimateMap[FaceDirection::FaceToWest] = createAnimateWithPlist(soldierConf->moveToWestAnimationPList, soldierConf->moveAnimateDelayPerUnit, GameObjectStatus::Move);
	
	//这里放开导致战争迷雾有问题，待确认
	m_dieAnimate = createAnimateWithPlist(soldierConf->dieAnimationPList, soldierConf->dieAnimateDelayPerUnit, GameObjectStatus::Die);
	
	m_standAnimateMap[FaceDirection::FaceToEast] = createAnimateWithPlist(soldierConf->standAndFaceToEastAnimationPList, soldierConf->standAnimateDelayPerUnit, GameObjectStatus::Stand);
	m_standAnimateMap[FaceDirection::FaceToNorthEast] = createAnimateWithPlist(soldierConf->standAndFaceToNorthEastAnimationPList, soldierConf->standAnimateDelayPerUnit, GameObjectStatus::Stand);
	m_standAnimateMap[FaceDirection::FaceToNorthWest] = createAnimateWithPlist(soldierConf->standAndFaceToNorthWestAnimationPList, soldierConf->standAnimateDelayPerUnit, GameObjectStatus::Stand);
	m_standAnimateMap[FaceDirection::FaceToSouthEast] = createAnimateWithPlist(soldierConf->standAndFaceToSouthEastAnimationPList, soldierConf->standAnimateDelayPerUnit, GameObjectStatus::Stand);
	m_standAnimateMap[FaceDirection::FaceToSouthWest] = createAnimateWithPlist(soldierConf->standAndFaceToSouthWestAnimationPList, soldierConf->standAnimateDelayPerUnit, GameObjectStatus::Stand);
	m_standAnimateMap[FaceDirection::FaceToWest] = createAnimateWithPlist(soldierConf->standAndFaceToWestAnimationPList, soldierConf->standAnimateDelayPerUnit, GameObjectStatus::Stand);
	
	float attackAnimateDelayPerUnit = 1.0f / (float)soldierConf->perSecondAttackCount;
	m_attackAnimateMap[FaceDirection::FaceToEast] = createAnimateWithPlist(soldierConf->attackToEastAnimationPList, attackAnimateDelayPerUnit, GameObjectStatus::Attack);
	m_attackAnimateMap[FaceDirection::FaceToNorthEast] = createAnimateWithPlist(soldierConf->attackToNorthEastAnimationPList, attackAnimateDelayPerUnit, GameObjectStatus::Attack);
	m_attackAnimateMap[FaceDirection::FaceToNorthWest] = createAnimateWithPlist(soldierConf->attackToNorthWestAnimationPList, attackAnimateDelayPerUnit, GameObjectStatus::Attack);
	m_attackAnimateMap[FaceDirection::FaceToSouthEast] = createAnimateWithPlist(soldierConf->attackToSouthEastAnimationPList, attackAnimateDelayPerUnit, GameObjectStatus::Attack);
	m_attackAnimateMap[FaceDirection::FaceToSouthWest] = createAnimateWithPlist(soldierConf->attackToSouthWestAnimationPList, attackAnimateDelayPerUnit, GameObjectStatus::Attack);
	m_attackAnimateMap[FaceDirection::FaceToWest] = createAnimateWithPlist(soldierConf->attackToWestAnimationPList, attackAnimateDelayPerUnit, GameObjectStatus::Attack);

	
	runAction(m_standAnimateMap[m_faceDirection]);

	return true;
}

bool Soldier::initData(SoldierType type)
{
	auto soldierConf = GameConfig::getInstance()->getSoldierConf(m_forceType, type);
	if (soldierConf == nullptr)
	{
		return false;
	}
	m_curHp = m_maxHp = soldierConf->maxHp;
	m_attackPower = soldierConf->attackPower;
	m_attackDistance = soldierConf->maxAttackRadius;
	m_alertDistance = soldierConf->maxAlertRadius;
	m_moveSpeed = soldierConf->perSecondMoveSpeedByPixel;
	m_attackPs = soldierConf->perSecondAttackCount;
	m_bulletType = soldierConf->bulletType;
	m_damageType = soldierConf->damageType;

	return true;
}

cocos2d::RepeatForever* Soldier::createAnimateWithPlist(const std::string& plistFile, float animateDelayPerUnit, GameObjectStatus status)
{
	auto animation = GameUtils::createAnimationWithPList(plistFile);
	auto& contentSize = getContentSize();
	if (contentSize.width == 0.0f || contentSize.height == 0.0f)
	{
		auto spriteFrame = animation->getFrames().at(0)->getSpriteFrame();
		setContentSize(spriteFrame->getOriginalSizeInPixels());
	}

	animation->setDelayPerUnit(animateDelayPerUnit);
	animation->setRestoreOriginalFrame(true);

	cocos2d::RepeatForever* repeatForeverAnimate = nullptr;
	switch (status)
	{
	case GameObjectStatus::Move:
	case GameObjectStatus::Stand:
	{
		repeatForeverAnimate = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
	}
	break;
	case GameObjectStatus::Attack:
	{
		auto animationEndFunc = cocos2d::CallFunc::create(CC_CALLBACK_0(Soldier::onAttackAnimationEnd, this));
		auto attackSequenceAction = cocos2d::Sequence::create(cocos2d::Animate::create(animation), animationEndFunc, nullptr);
		repeatForeverAnimate =cocos2d::RepeatForever::create(attackSequenceAction);
	}
	break;
	case GameObjectStatus::Die:
	{
		auto animationEndFunc = cocos2d::CallFunc::create(CC_CALLBACK_0(Soldier::onDieAnimationEnd, this));
		auto dieSequenceAction = cocos2d::Sequence::create(cocos2d::Animate::create(animation), animationEndFunc, nullptr);
		repeatForeverAnimate = cocos2d::RepeatForever::create(dieSequenceAction);

		m_dieAnimationFrameSize = animation->getFrames().at(0)->getSpriteFrame()->getOriginalSizeInPixels();
	}
	break;
	default:    
		break;
	}

	repeatForeverAnimate->retain();

	return repeatForeverAnimate;
}

void Soldier::onAttackAnimationEnd()
{
	if (m_bulletType != BulletType::Invalid)
	{
		auto bullet = BulletManager::getInstance()->createBullet(m_bulletType, m_uniqId, m_enemyId);
		if (bullet != nullptr)
		{
			MapManager::getInstance()->addChildToGameObjectLayer(bullet);
		}
	}
	else
	{
		auto enemy = GameObjectManager::getInstance()->getGameObjectById(m_enemyId);
		if (enemy && !enemy->isReadyToRemove())
		{
			enemy->reduceHP(getAttackPower());
		}
	}

	//SoundManager::getInstance()->playNpcEffect(_templateName, NpcSoundEffectType::Attack);
}

void Soldier::onDieAnimationEnd()
{
	GameObjectManager::getInstance()->addReadyToRemoveGameObject(m_uniqId);
}

void Soldier::moveTo(const cocos2d::Vec2& pos)
{
	cocos2d::log("soldier:%d will move to:[%0.1f, %0.1f]",m_uniqId, pos.x, pos.y);
	//接收到新的目的点，清空之前的寻路路径
	if (!m_pathList.empty())
	{
		m_pathList.clear();
	}
	auto endTileNode = MapManager::getInstance()->getTileNode(pos);
	auto curPos = this->getPosition();
	auto curTileNode = MapManager::getInstance()->getTileNode(curPos);
	cocos2d::log("from tile node:[%d, %d] to [%d, %d]", curTileNode->rowIndex, curTileNode->columnIndex, 
		endTileNode->rowIndex, endTileNode->columnIndex);
	m_pathList = AutoFindPath::computeTileNodePathListBetween(curTileNode, endTileNode);
	
	cocos2d::log("find path, node size:%lu", m_pathList.size());
	toMove();
}


void Soldier::update(float deltaTime)
{
	GameObject::update(deltaTime);
	//GameBattle里面已经控制指定攻击目标，之类只需要定时刷新状态，以及行进过程中的遭遇战
	//if (m_forceType == ForceType::AI)
	{
		findAndFight(deltaTime);
	}
	
}

void Soldier::attackTarget(GameObject * target)
{
	m_attackTarget = target;
	//是否在攻击距离内
	if (isEnemyInAttackRange(target))
	{
		toAttack();
	}
	else
	{
		moveTo(target->getPosition());
	}
}

void Soldier::toStand()
{
	if (m_objectStatus == GameObjectStatus::Stand)
	{
		return;
	}
	if (!m_pathList.empty())
	{
		m_pathList.clear();
	}
	stopAllActions();
	m_objectStatus = GameObjectStatus::Stand;
	runAction(m_standAnimateMap[m_faceDirection]);
}

void Soldier::toMove()
{
	if (m_pathList.empty())
	{
		return;
	}
	TileNode* curNode = m_pathList.front();
	m_pathList.pop_front();
	cocos2d::log("-->[%d, %d]==>[%0.1f, %0.1f]", curNode->rowIndex, curNode->columnIndex, curNode->position.x, curNode->position.y);
	FaceDirection faceDirection = getFaceDirection(curNode->position);
	auto animteIt = m_moveAnimateMap.find(faceDirection);
	if (animteIt != m_moveAnimateMap.end())
	{
		if ((m_objectStatus != GameObjectStatus::Move) || (faceDirection != m_faceDirection))
		{
			m_objectStatus = GameObjectStatus::Move;
			stopAllActions();
			m_faceDirection = faceDirection;
			runAction(animteIt->second);
		}
		
		auto moveDuration = getMoveToDuration(curNode->position);
		auto moveDelta = curNode->position - getPosition();
		auto moveBy = cocos2d::MoveBy::create(moveDuration, moveDelta);
		cocos2d::CallFunc* moveEndEvent = nullptr;
		if (m_pathList.empty())
		{
			moveEndEvent = cocos2d::CallFunc::create(CC_CALLBACK_0(Soldier::toStand, this));
		}
		else
		{
			moveEndEvent = cocos2d::CallFunc::create(CC_CALLBACK_0(Soldier::toMove, this));
		}
		auto sequenceAction = cocos2d::Sequence::create(moveBy, moveEndEvent, nullptr);
		runAction(sequenceAction);
		
	}
}

void Soldier::toAttack()
{
	if (m_objectStatus == GameObjectStatus::Attack)
	{
		return;
	}
	if (!m_pathList.empty())
	{
		m_pathList.clear();
	}
	stopAllActions();
	runAction(m_attackAnimateMap[m_faceDirection]);
	m_objectStatus = GameObjectStatus::Attack;
}

void Soldier::toDie()
{
	//SoundManager::getInstance()->playNpcEffect(_templateName, NpcSoundEffectType::Death);
	m_soldierStatus == GameObjectStatus::Die;
    stopAllActions();
    runAction(m_dieAnimate);

}

FaceDirection Soldier::getFaceDirection(const cocos2d::Vec2& moveToPos)
{
	FaceDirection faceToDirection = FaceDirection::Invalid;
	auto position = getPosition();

	float degree = GameUtils::computeRotatedDegree(position, moveToPos);
	if (degree >= 0.0f && degree < 60.0f)
	{
		faceToDirection = FaceDirection::FaceToNorthEast;
	}
	else if (degree >= 60.0f && degree < 120.0f)
	{
		faceToDirection = FaceDirection::FaceToEast;
	}
	else if (degree >= 120.0f && degree < 180.0f)
	{
		faceToDirection = FaceDirection::FaceToSouthEast;
	}
	else if (degree >= 180.0f && degree < 240.0f)
	{
		faceToDirection = FaceDirection::FaceToSouthWest;
	}
	else if (degree >= 240.0f && degree < 300.0f)
	{
		faceToDirection = FaceDirection::FaceToWest;
	}
	else
	{
		faceToDirection = FaceDirection::FaceToNorthWest;
	}

	return faceToDirection;
}

float Soldier::getMoveToDuration(const cocos2d::Vec2& moveToPos)
{
	auto position = getPosition();
	float distance = std::sqrt((moveToPos.x - position.x) * (moveToPos.x - position.x) +
		(moveToPos.y - position.y) * (moveToPos.y - position.y));

	return distance / m_moveSpeed;
}

bool Soldier::isEnemyDiappear(GameObject * enemy)
{
	if ((enemy == nullptr) || (enemy->isReadyToRemove()))
	{
		return true;
	}
	return false;
}

bool Soldier::isReadyToRemove()
{
	return m_soldierStatus == GameObjectStatus::Die;
}

void Soldier::onPrepareToRemove()
{
	toDie();
}

void Soldier::initHpBar()
{
	if (m_forceType == ForceType::Player)
	{
		m_hpBar = cocos2d::ui::LoadingBar::create(PLAYER_HP_BAR_TEXTURE_NAME);
	}
	else
	{
		m_hpBar = cocos2d::ui::LoadingBar::create(AI_HP_BAR_TEXTURE_NAME);
	}

	m_hpBar->setAnchorPoint(cocos2d::Vec2::ZERO);
	m_hpBar->setPercent(100.0f);

	auto hpBarBackground = Sprite::create(HP_BAR_BACKGROUND_TEXTURE_NAME);
	hpBarBackground->setCascadeOpacityEnabled(true);
	hpBarBackground->setScale(0.5f);
	hpBarBackground->addChild(m_hpBar);
	hpBarBackground->setVisible(true);

	addChild(hpBarBackground);

	auto contentSize = getContentSize();
	hpBarBackground->setPosition(cocos2d::Vec2(contentSize.width / 2.0, contentSize.height + 10));
}

void Soldier::findAndFight(float deltaTime)
{
	static bool moveToEnemy = false;
	m_alertTimeCd += deltaTime;
	if (m_enemyId != INVALID_GAMEOBJECT_ID)
	{
		auto enemy = GameObjectManager::getInstance()->getGameObjectById(m_enemyId);
		if (isEnemyDiappear(enemy))
		{
			moveToEnemy = false;
			m_enemyId = INVALID_GAMEOBJECT_ID;
			/*if (m_attackTarget != nullptr)
			{
				attackTarget(m_attackTarget);
			}*/
			toStand();
		}
		//在攻击范围内
		else if (isEnemyInAttackRange(enemy))
		{
			moveToEnemy = false;
			cocos2d::log(">>>> attack enemy:%d", enemy->getId());
			toAttack();
		}
		//在警戒范围内，不在攻击范围内
		else
		{
			cocos2d::log(">>>> in alert range, move to enemy:%d", enemy->getId());
			if (!moveToEnemy)
			{
				moveTo(enemy->getPosition());
				moveToEnemy = true;
			}
			
		}
	}
	else
	{
		moveToEnemy = false;
		if (m_alertTimeCd >= GameConfig::getInstance()->getCooldownConf()->alertCdTime)
		{
			searchEnemy();
			m_alertTimeCd = 0;
		}
	}
}

void Soldier::searchEnemy()
{
	auto& gameObjects = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto& gameObjectIt : gameObjects)
	{
		auto gameObject = gameObjectIt.second;
		if (m_forceType == gameObject->getForceType())
		{
			continue;
		}
		if (isEnemyInAlertRange(gameObject))
		{
			cocos2d::log(">>>> find enemy:%d", gameObject->getId());
			setEnemyId(gameObject->getId());
		}
	}
}

bool Soldier::isEnemyInAlertRange(GameObject * enemy)
{
	auto& startPos = this->getPosition();
	auto enemyPos = getEnemyPosition(enemy);
	auto distance = GameUtils::computeDistanceBetween(startPos, enemyPos);
	if (distance <= m_alertDistance)
	{
		return true;
	}
	return false;
}


cocos2d::Vec2 Soldier::getEnemyPosition(GameObject * enemy)
{
	cocos2d::Vec2 targetPos;
	if (enemy->getGameObjectType() == GameObjectType::Soldier)
	{
		targetPos = enemy->getPosition();
	}
	else if (enemy->getGameObjectType() == GameObjectType::Building)
	{
		float minDistance = FLT_MAX;
		auto& startPos = this->getPosition();
		Building* building = dynamic_cast<Building*>(enemy);
		//建筑占用的格子较多，这里找出距离最近的格子
		auto& bottonGridPos = building->getBottonGirdPos();
		for (auto& endPos : bottonGridPos)
		{
			auto distance = GameUtils::computeDistanceBetween(startPos, endPos);
			if (distance < minDistance)
			{
				minDistance = distance;
				targetPos = endPos;
			}
		}
	}
	return targetPos;
}

bool Soldier::isEnemyInAttackRange(GameObject * enemy)
{
	auto startPos = getPosition();
	auto targetPos = getEnemyPosition(enemy);
	auto distance = GameUtils::computeDistanceBetween(startPos, targetPos);
	if (distance <= m_attackDistance)
	{
		return true;
	}
	return false;
}
