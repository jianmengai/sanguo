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
#include "TeamManager.h"

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
	initTeamText();

	m_uniqId = GameUtils::getLastestUniqId();

	toStand();
	
	scheduleUpdate();
	
	//敌方单位默认不显示
	if (m_forceType == ForceType::AI)
	{
		setVisible(false);
	}
	
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
	m_curSpeed = m_moveSpeed;
	m_attackPs = soldierConf->perSecondAttackCount;
	m_bulletType = soldierConf->bulletType;
	m_damageType = soldierConf->damageType;

	m_soldierType = type;

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

		//m_dieAnimationFrameSize = animation->getFrames().at(0)->getSpriteFrame()->getOriginalSizeInPixels();
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
	
	SoundManager::getInstance()->playNpcEffect(m_soldierType, NpcSoundEffectType::Attack);
}

void Soldier::onDieAnimationEnd()
{
	cocos2d::log("will remove id:%d", m_uniqId);
	stopAllActions();
	GameObjectManager::getInstance()->addReadyToRemoveGameObject(m_uniqId);
}

void Soldier::moveTo(const cocos2d::Vec2& pos)
{
	if (m_objectStatus == GameObjectStatus::Die)
	{
		return;
	}
	
	auto toRowCol = MapManager::getInstance()->toTileRowCol(pos);
	if ((toRowCol == m_moveToPosRowCol) )
	{
		return;
	}
	cocos2d::log("soldier:%d will move to:[%0.1f, %0.1f]", m_uniqId, pos.x, pos.y);
	m_moveToPosRowCol = toRowCol;
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
			
	//stopAllActions();
	toMove();
}

void Soldier::setTeamSpeed(float speed)
{
	m_curSpeed = speed;
}

void Soldier::resumeNormalSpeed()
{
	m_curSpeed = m_moveSpeed;
}




void Soldier::update(float deltaTime)
{
	if (isReadyToRemove())
	{
		return;
	}
	GameObject::update(deltaTime);
	//GameBattle里面已经控制指定攻击目标，之类只需要定时刷新状态，以及行进过程中的遭遇战
	//if (m_forceType == ForceType::AI)
	{
		findAndFight(deltaTime);
	}
	movePath();
	updateMove();
}


void Soldier::attackTarget(GameObject * target)
{
	if (m_attackTarget == target)
	{
		return;
	}
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
	clearMoveToRowCol();
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
	if (m_pathList.empty())
	{
		curNode = getLastNode(curNode);
	}
	//cocos2d::log("-->[%d, %d]==>[%0.1f, %0.1f]", curNode->rowIndex, curNode->columnIndex, curNode->position.x, curNode->position.y);
	FaceDirection faceDirection = getFaceDirection(curNode->position);
	//cocos2d::log("face direction:%d", faceDirection);
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
		
		const auto& curPosition = getPosition();
		auto moveDuration = getMoveToDuration(curPosition, curNode->position);
		//cocos2d::log("-->move duration:%0.1f", moveDuration);
		auto moveDelta = curNode->position - curPosition;
		auto moveBy = cocos2d::MoveBy::create(moveDuration, moveDelta);
		cocos2d::CallFunc* moveEndEvent = nullptr;
		if (m_pathList.empty())
		{
			cocos2d::log("move to stand...");
			moveEndEvent = cocos2d::CallFunc::create(CC_CALLBACK_0(Soldier::toStand, this));
		}
		else
		{
			moveEndEvent = cocos2d::CallFunc::create(CC_CALLBACK_0(Soldier::toMove, this));
		}
		//auto spawn = cocos2d::Spawn::createWithTwoActions(moveBy, animteIt->second);
		auto sequenceAction = cocos2d::Sequence::create(moveBy, moveEndEvent, nullptr);
		runAction(sequenceAction);
	}
}

void Soldier::toAttack()
{
	clearMoveToRowCol();
	if (m_objectStatus == GameObjectStatus::Attack)
	{
		return;
	}
	if (!m_pathList.empty())
	{
		m_pathList.clear();
	}
	stopAllActions();
	if (m_attackTarget != nullptr)
	{
		auto faceDirection = getFaceDirection(m_attackTarget->getPosition());
		if (faceDirection != m_faceDirection)
		{
			m_faceDirection = faceDirection;
		}
	}
	cocos2d::log("run attack action");
	runAction(m_attackAnimateMap[m_faceDirection]);
	m_objectStatus = GameObjectStatus::Attack;
}

void Soldier::toDie()
{
	clearMoveToRowCol();
	SoundManager::getInstance()->playNpcEffect(m_soldierType, NpcSoundEffectType::Death);
	m_objectStatus = GameObjectStatus::Die;
    stopAllActions();
	cocos2d::log("run die action...");
    runAction(m_dieAnimate);

}

void Soldier::clearMoveToRowCol()
{
	m_moveToPosRowCol.x = 0;
	m_moveToPosRowCol.y = 0;
}

void Soldier::updatePreparePath()
{

}


void Soldier::movePath()
{
	if (m_preparePathList.empty())
	{
		return;
	}
	auto& pos = m_preparePathList.front();
	moveTo(pos);
	//已经到达当前位置
	if (getPosition() == pos)
	{
		m_preparePathList.pop_front();
	}
	
}

void Soldier::updateMove()
{
	if (!m_pathList.empty() && (m_objectStatus == GameObjectStatus::Stand))
	{
		toMove();
	}
}

TileNode * Soldier::getLastNode(TileNode * node)
{
	if (node == nullptr)
	{
		return nullptr;
	}
	//非移动单位占用
	if (node->occupy != OccupyType::Soldier)
	{
		return node;
	}
	TileNode* lastNode = node;
	int y = node->columnIndex;
	int x = node->rowIndex;
	auto size = MapManager::getInstance()->getMapSize();
	int maxX = size.height;
	int maxY = size.width;
	for (int i = 1; i < 100; ++i)
	{
		for (int curX = x - i; curX <= x + i; ++curX)
		{
			for (int curY = y - i; curY <= y + i; ++curY)
			{
				if ((curX == x) && (curY == y))
				{
					continue;
				}
				if ((curX < 0) || (curX >= maxX))
				{
					continue;
				}
				if ((curY < 0) || (curY >= maxY))
				{
					continue;
				}
				if (MapManager::getInstance()->checkOccupy(curX, curY, OccupyType::Soldier))
				{
					lastNode = MapManager::getInstance()->getTileNode(curX, curY);
					return lastNode;
				}
			}
		}
	}
	return lastNode;
}



const cocos2d::Vec2& Soldier::getPosition() const
{
	if (m_inDefenceTower)
	{
		auto parent = getParent();
		auto selfPos = GameObject::getPosition();
		auto parentPos = parent->getPosition();
		auto worldPos = convertToWorldSpace(selfPos);
		static auto pos = cocos2d::Vec2::ZERO;
		pos = parent->getPosition() + cocos2d::Vec2(0, 100);
		return pos;
	}
	else
	{
		return GameObject::getPosition();
	}
}

void Soldier::setAttackDistance(float distance)
{
	m_attackDistance = distance;
}

void Soldier::setAttackPower(int power)
{
	m_attackPower = power;
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

float Soldier::getMoveToDuration(const cocos2d::Vec2& startPos, const cocos2d::Vec2& moveToPos)
{
	float distance = std::sqrt((moveToPos.x - startPos.x) * (moveToPos.x - startPos.x) +
		(moveToPos.y - startPos.y) * (moveToPos.y - startPos.y));
	//cocos2d::log("-----> start:[%0.1f, %0.1f] -> end:[%0.1f, %0.1f], distance:%0.1f", startPos.x, startPos.y, moveToPos.x, moveToPos.y, distance);
	return distance / m_curSpeed;
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
	return m_objectStatus == GameObjectStatus::Die;
}

void Soldier::setPath(std::list<cocos2d::Vec2>& path)
{
	m_preparePathList = path;
}

void Soldier::setTeamNo(TeamNo teamNo)
{
	m_teamNo = teamNo;
	if (m_teamNo == TeamNo::Invalid)
	{
		TeamManager::getInstance()->removeFromTeam(this);
		m_teamText->setVisible(false);
		resumeNormalSpeed();
	}
	if (m_forceType == ForceType::AI)
	{
		return;
	}
	std::string teamName;
	switch (m_teamNo)
	{
	case TeamNo::One:
	{
		teamName = "1";
	}
	break;
	case TeamNo::Two:
	{
		teamName = "2";
	}
	break;
	case TeamNo::Three:
	{
		teamName = "3";
	}
	break;
	case TeamNo::Four:
	{
		teamName = "4";
	}
	break;
	case TeamNo::Five:
	{
		teamName = "5";
	}
	break;
	case TeamNo::Six:
	{
		teamName = "6";
	}
	break;
	case TeamNo::Seven:
	{
		teamName = "7";
	}
	break;
	case TeamNo::Eight:
	{
		teamName = "8";
	}
	break;
	case TeamNo::Nine:
	{
		teamName = "9";
	}
	break;
	default:
		break;
	}
	if (!teamName.empty())
	{
		m_teamText->setVisible(true);
		m_teamText->setString(teamName);
	}
}

TeamNo Soldier::getTeamNo()
{
	return m_teamNo;
}

SoldierType Soldier::getSoldierType()
{
	return m_soldierType;
}

void Soldier::inDefenceTower(bool in)
{
	m_inDefenceTower = in;
	if (m_inDefenceTower)
	{
		m_canBeAttacked = false;
	}
}


void Soldier::onPrepareToRemove()
{
	cocos2d::log("onPrepareToRemove  to die");
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
	auto contentSize = getContentSize();
	if (contentSize.height == 150)
	{
		contentSize.height = 120;
	}
	hpBarBackground->setPosition(cocos2d::Vec2(contentSize.width / 2.0, contentSize.height + 10));
	addChild(hpBarBackground);
}

void Soldier::initTeamText()
{
	m_teamText = cocos2d::ui::Text::create();
	m_teamText->setVisible(false);
	auto contentSize = getContentSize();
	if (contentSize.height == 150)
	{
		contentSize.height = 120;
	}
	m_teamText->setPosition(cocos2d::Vec2(contentSize.width / 2.0, contentSize.height + 30));
	m_teamText->setFontSize(30);
	addChild(m_teamText);
}

TileNode * Soldier::adjustEndNode(TileNode * endNode)
{
	if (endNode == nullptr)
	{
		return nullptr;
	}


	return nullptr;
}

void Soldier::findAndFight(float deltaTime)
{
	m_alertTimeCd += deltaTime;
	if (m_enemyId != INVALID_GAMEOBJECT_ID)
	{
		auto enemy = GameObjectManager::getInstance()->getGameObjectById(m_enemyId);
		if (isEnemyDiappear(enemy))
		{
			m_moveToEnemy = false;
			m_attacking = false;
			m_enemyId = INVALID_GAMEOBJECT_ID;
			m_attackTarget = nullptr;
			/*if (m_attackTarget != nullptr)
			{
				attackTarget(m_attackTarget);
			}*/
			cocos2d::log("enemy disappear, stand");
			toStand();
		}
		//在攻击范围内
		else if (isEnemyInAttackRange(enemy))
		{
			m_moveToEnemy = false;
			//cocos2d::log(">>>> attack enemy:%d", enemy->getId());
			if (!m_attacking)
			{
				cocos2d::log("attack enemy:%d", m_enemyId);
				toAttack();
				m_attacking = true;
			}
			
		}
		//在警戒范围内，不在攻击范围内
		else if(!m_inDefenceTower)
		{
			//cocos2d::log(">>>> in alert range, move to enemy:%d", enemy->getId());
			if (!m_moveToEnemy)
			{
				cocos2d::log("move to enemy:%d", m_enemyId);
				moveTo(enemy->getPosition());
				m_moveToEnemy = true;
			}
			
		}
	}
	//else
	{
		if (m_alertTimeCd >= GameConfig::getInstance()->getCooldownConf()->alertCdTime)
		{
			GameObject* object = nullptr;
			if (m_forceType == ForceType::AI)
			{
				object = searchEnemy();
			}
			//所有周围alter范围内的目标
			else
			{
				object = searchNearbyEnemy();
			}
			if ((object != m_attackTarget) && (object != nullptr))
			{
				m_attackTarget = object;
				setEnemyId(object->getId());
				m_moveToEnemy = false;
				m_attacking = false;
			}
			m_alertTimeCd = 0;
		}
	}
	
}

GameObject* Soldier::searchEnemy()
{
	clock_t start = clock();
	GameObject* nearbyObject = nullptr;
	float distance = 0;
	auto& gameObjects = GameObjectManager::getInstance()->getGameObjectMap();
	std::map<float, std::vector<GameObject*> > buildingDistance;
	std::map<float, std::vector<GameObject*> > soldierDistance;
	std::vector<GameObject*> alertDistance;
	GameObject* enemy = nullptr;
	for (auto& gameObjectIt : gameObjects)
	{
		auto gameObject = gameObjectIt.second;
		if ((gameObject == nullptr) || (m_forceType == gameObject->getForceType()) || gameObject->isReadyToRemove() || (!gameObject->canBeAttacked()))
		{
			continue;
		}
		distance = getEnemyDistance(gameObject);
		//在警戒范围内，直接选定该目标攻击
		if ((distance <= m_alertDistance) || (distance <= m_attackDistance))
		{
			alertDistance.push_back(gameObject);
		}
		auto type = gameObject->getGameObjectType();
		if (type == GameObjectType::Building)
		{
			buildingDistance[distance].push_back(gameObject);
		}
		else if (type == GameObjectType::Soldier)
		{
			soldierDistance[distance].push_back(gameObject);
		}
	}
	if (!alertDistance.empty())
	{
		auto size = alertDistance.size();
		auto index = rand() % size;
		enemy = alertDistance[index];
	}
	if (enemy == nullptr)
	{
		if (!buildingDistance.empty())
		{
			auto it = buildingDistance.begin();
			auto buildings = it->second;
			enemy = buildings[0];
		}
		else if (!soldierDistance.empty())
		{
			auto it = soldierDistance.begin();
			auto soldiers = it->second;
			enemy = soldiers[0];
		}
	}
	/*
	for (auto pairs : enemyDistance)
	{
		//在警戒距离内
		if (pairs.first <= m_alertDistance)
		{
			for (auto object : pairs.second)
			{
				//cocos2d::log("in alert range, distance:%0.1f, alert:%0.1f, id:%d", pairs.first, m_alertDistance, object->getId());
				nearbyObject = object;
				break;
			}
			if (nearbyObject != nullptr)
			{
				break;
			}
		}
		else
		{
			//先找建筑单位
			for (auto object : pairs.second)
			{
				if (object->getGameObjectType() == GameObjectType::Building)
				{
					//cocos2d::log("nearby building, distance:%0.1f, alert:%0.1f, id:%d", pairs.first, m_alertDistance, object->getId());
					nearbyObject = object;
					break;
				}
			}
			if (nearbyObject != nullptr)
			{
				break;
			}
		}
	}
	
	if (m_attackTarget == nullptr)
	{
		for (auto pairs : enemyDistance)
		{
			for (auto object : pairs.second)
			{
				//cocos2d::log("nearby soldier, distance:%0.1f, alert:%0.1f, id:%d", pairs.first, m_alertDistance, object->getId());

				nearbyObject = object;
				break;
			}
			if (nearbyObject != nullptr)
			{
				break;
			}
		}
	}
	*/
	nearbyObject = enemy;
	clock_t end = clock();
	cocos2d::log("====================npc find target %u", end - start);
	return nearbyObject;
}

GameObject* Soldier::searchNearbyEnemy()
{
	GameObject* nearbyObject = nullptr;
	float minDistance = FLT_MAX;
	float distance = 0;
	auto& gameObjects = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto& gameObjectIt : gameObjects)
	{
		auto gameObject = gameObjectIt.second;
		if ((gameObject == nullptr) || (m_forceType == gameObject->getForceType()) || gameObject->isReadyToRemove() || (!gameObject->canBeAttacked()))
		{
			continue;
		}
		distance = getEnemyDistance(gameObject);
		if (distance < minDistance)
		{
			minDistance = distance;
			nearbyObject = gameObject;
		}
		
	}
	if ((minDistance <= m_alertDistance) || (minDistance <= m_attackDistance))
	{
		return nearbyObject;
	}

	return nullptr;
}

bool Soldier::isEnemyInAlertRange(GameObject * enemy, float& distance)
{
	auto& startPos = this->getPosition();
	auto enemyPos = getEnemyPosition(enemy);
	distance = GameUtils::computeDistanceBetween(startPos, enemyPos);
	if (distance <= m_alertDistance)
	{
		return true;
	}
	return false;
}

float Soldier::getEnemyDistance(GameObject * enemy)
{
	auto& startPos = this->getPosition();
	auto enemyPos = getEnemyPosition(enemy);
	return GameUtils::computeDistanceBetween(startPos, enemyPos);
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

cocos2d::Vec2 Soldier::getEnemyNearByPosition(cocos2d::Vec2 & enemyPosition)
{
	auto faceDirection = getFaceDirection(enemyPosition);
	auto tileNodeRowCol = MapManager::getInstance()->toTileRowCol(enemyPosition);
	switch (faceDirection)
	{
	case FaceDirection::FaceToEast:
	{
		++tileNodeRowCol.x;
		--tileNodeRowCol.y;
	}
	break;
	case FaceDirection::FaceToNorthEast:
	{
		--tileNodeRowCol.x;
	}
	break;
	case FaceDirection::FaceToNorthWest:
	{
		--tileNodeRowCol.y;
	}
	break;
	case FaceDirection::FaceToSouthEast:
	{

	}
	break;
	case FaceDirection::FaceToSouthWest:
	{

	}
	break;
	case FaceDirection::FaceToWest:
	{

	}
	break;
	default:
	{

	}
	break;
	}
	return cocos2d::Vec2();
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
