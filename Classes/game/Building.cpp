#include "Building.h"
#include "GameConfig.h"
#include "MapManager.h"
#include "GameUtils.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "Soldier.h"



Building* Building::create(ForceType forceType, BuildingType type, const cocos2d::Vec2& position)
{
	auto building = new Building();
	if (building && building->init(forceType, type, position))
	{
		building->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(building);
	}
	
	return building;
}

bool Building::init(ForceType forceType, const BuildingType type, const cocos2d::Vec2& position)
{
	if (!GameObject::init())
	{
		return false;
	}
	m_forceType = forceType;
	m_objectType = GameObjectType::Building;
	m_buildingStatus = BuildingStatus::PrepareToBuild;
	auto rowCol = MapManager::getInstance()->toTileRowCol(position);
	cocos2d::log("==================row:%0.1f, col:%0.1f===============", rowCol.x, rowCol.y);
	this->setPosition(position);
	m_bottonGridPos.push_back(position);
	initBuildingStatusSprites(type);
	initBottomGridSprites(type);
	initData(type);
	setScale(1.5f);
	m_uniqId = GameUtils::getLastestUniqId();
	m_buildingType = type;
	
	auto cs = getContentSize();
	initHpBar();
	initBeingBuiltProgressBar();

	scheduleUpdate();
	updateStatus(BuildingStatus::BeingBuilt);

	//敌方单位默认不显示
	if (m_forceType == ForceType::AI)
	{
		setVisible(false);
	}
	
	return true;
}

void Building::initBuildingStatusSprites(const BuildingType type)
{
	createBuildingStatusSprite(type, BuildingStatus::PrepareToBuild, 180);
	createBuildingStatusSprite(type, BuildingStatus::BeingBuilt);
	createBuildingStatusSprite(type, BuildingStatus::Working);
	createBuildingStatusSprite(type, BuildingStatus::Destory);
}



cocos2d::Sprite* Building::createBuildingStatusSprite(const BuildingType type, BuildingStatus buildingStatus, int opacity /* = 255 */)
{
	auto buildingConf = GameConfig::getInstance()->getBuildingConf(m_forceType, type);
	if (buildingConf == nullptr)
	{
		return nullptr;
	}
	float shadowYPosition = 0.0f;
	std::string spriteTextureName;
	bool hasShadow = false;

	switch (buildingStatus)
	{
	case BuildingStatus::PrepareToBuild:
		spriteTextureName = buildingConf->prepareToBuildStatusTextureName;
		break;
	case BuildingStatus::BeingBuilt:
		shadowYPosition = buildingConf->shadowYPositionInBeingBuiltStatus;
		spriteTextureName = buildingConf->beingBuiltStatusTextureName;
		hasShadow = true;
		break;
	case BuildingStatus::Working:
		shadowYPosition = buildingConf->shadowYPositionInWorkingStatus;
		spriteTextureName = buildingConf->workingStatusTextureName.c_str();
		hasShadow = true;
		break;
	case BuildingStatus::Destory:
		shadowYPosition = buildingConf->shadowYPositionInDestroyStatus;
		spriteTextureName = buildingConf->destroyStatusTextureName;
		hasShadow = true;
		break;
	default:    
		break;
	}

	auto buildingStatusSpriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteTextureName);
	auto buildingStatusSprite = Sprite::create();
	//Sprite::createWithSpriteFrameName();
	buildingStatusSprite->setSpriteFrame(buildingStatusSpriteFrame);
	buildingStatusSprite->setOpacity(opacity);
	this->addChild(buildingStatusSprite);
	m_buildingStatusSpriteMap[buildingStatus] = buildingStatusSprite;

	/*if (hasShadow)
	{
		auto buildStatusSpriteSize = buildingStatusSprite->getContentSize();

		auto shadowSprite = Sprite::create();
		auto shadowSpriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(buildingConf->shadowTextureName);
		shadowSprite->setSpriteFrame(shadowSpriteFrame);
		shadowSprite->setPosition(cocos2d::Vec2(buildStatusSpriteSize.width / 2.0f, shadowYPosition));
		buildingStatusSprite->addChild(shadowSprite, -1);
	}*/

	return buildingStatusSprite;
}


void Building::initBottomGridSprites(const BuildingType type)
{
	auto buildingConf = GameConfig::getInstance()->getBuildingConf(m_forceType, type);
	if (buildingConf == nullptr)
	{
		return ;
	}

	auto& tileSize = MapManager::getInstance()->getTileSize();

	auto prepareToBuildSprite = m_buildingStatusSpriteMap[BuildingStatus::PrepareToBuild];
	auto prepareToBuildSpriteSize = prepareToBuildSprite->getContentSize();

	cocos2d::Vec2 centerBottomGridPosition(prepareToBuildSprite->getContentSize().width / 2.0f, buildingConf->centerBottomGridYPosition);
	cocos2d::Vec2 firstQueryGridPosition(centerBottomGridPosition.x, centerBottomGridPosition.y + tileSize.height);
	m_bottomGridsPlaneCenterPositionInLocalSpace = centerBottomGridPosition;

	for (int i = 0; i < buildingConf->bottomGridRowCount; ++i)
	{
		cocos2d::Vec2 tempGridPostion;
		tempGridPostion.x = firstQueryGridPosition.x - i * tileSize.width / 2.0f;
		tempGridPostion.y = firstQueryGridPosition.y - i * tileSize.height / 2.0f;

		for (int j = 0; j < buildingConf->bottomGridColumnCount; ++j)
		{
			auto gridSprite = Sprite::create();
			auto gridSpriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(ENABLE_BUILD_GRID_FILE_NAME);
			gridSprite->setSpriteFrame(gridSpriteFrame);
			gridSprite->setPosition(tempGridPostion);
			prepareToBuildSprite->addChild(gridSprite, -1);

			m_bottomGridSpritesList.push_back(gridSprite);

			tempGridPostion.x += tileSize.width / 2.0f;
			tempGridPostion.y -= tileSize.height / 2.0f;
		}
	}
}

void Building::initData(const BuildingType type)
{
	auto buildingConf = GameConfig::getInstance()->getBuildingConf(m_forceType, type);
	if (buildingConf == nullptr)
	{
		return ;
	}
	m_curHp = m_maxHp = buildingConf->maxHP;
	m_buildingTimeBySecond = buildingConf->buildingTimeBySecond;
	m_attackPower = buildingConf->attackPower;
	m_attackDistance = buildingConf->attackRange;
}

bool Building::isReadyToRemove()
{
	return m_buildingStatus == BuildingStatus::Destory;
}

bool Building::isWorking()
{
	return m_buildingStatus == BuildingStatus::Working;
}

void Building::onPrepareToRemove()
{
	updateStatus(BuildingStatus::Destory);
}

void Building::initHpBar()
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

	auto contentSize = m_buildingStatusSpriteMap[BuildingStatus::Working]->getContentSize();
	auto hpContentSize = hpBarBackground->getContentSize();
	auto scaleX = contentSize.width / hpContentSize.width;
	hpBarBackground->setScaleX(scaleX);
	hpBarBackground->setPosition(cocos2d::Vec2(contentSize.width / 2.0, contentSize.height + 20));
}

const cocos2d::Size& Building::getContentSize()
{
	auto& prepareToBuildSprite = m_buildingStatusSpriteMap[BuildingStatus::PrepareToBuild];
	return prepareToBuildSprite->getContentSize();
}

BuildingType Building::getBuildingType()
{
	return m_buildingType;
}

BuildingStatus Building::getBuildingStatus()
{
	return m_buildingStatus;
}

std::vector<cocos2d::Vec2>& Building::getBottonGirdPos()
{
	// TODO: 在此处插入 return 语句
	return m_bottonGridPos;
}

void Building::update(float deltaTime)
{
	GameObject::update(deltaTime);
	if (m_buildingStatus == BuildingStatus::BeingBuilt)
	{
		hideHpBar();
		updateBeingBuiltProgressBar(deltaTime);
	}
}


void Building::initBeingBuiltProgressBar()
{
	m_beingBuildProgressBar = cocos2d::ui::LoadingBar::create(BEING_BUILT_PROGRESS_BAR);
	m_beingBuildProgressBar->setAnchorPoint(cocos2d::Vec2::ZERO);
	m_beingBuildProgressBar->setPercent(0.0f);

	auto beingBuiltProgressBarBackground = Sprite::create(HP_BAR_BACKGROUND_TEXTURE_NAME);
	beingBuiltProgressBarBackground->setCascadeOpacityEnabled(true);
	beingBuiltProgressBarBackground->setScale(0.5f);
	beingBuiltProgressBarBackground->addChild(m_beingBuildProgressBar);
	beingBuiltProgressBarBackground->setVisible(false);
	addChild(beingBuiltProgressBarBackground);

	auto barContentSize = beingBuiltProgressBarBackground->getContentSize();
	auto contentSize = m_buildingStatusSpriteMap[BuildingStatus::BeingBuilt]->getContentSize();
	auto scaleX = contentSize.width / barContentSize.width;
	beingBuiltProgressBarBackground->setScaleX(scaleX);
	beingBuiltProgressBarBackground->setPosition(cocos2d::Vec2(contentSize.width / 2.0, contentSize.height));
}

void Building::updateBeingBuiltProgressBar(float deltaTime)
{
	auto percent = m_passTimeBySecondInBeingBuiltStatus / m_buildingTimeBySecond * 100.0f;
	m_beingBuildProgressBar->setPercent(percent);
	m_passTimeBySecondInBeingBuiltStatus += deltaTime;
}

void Building::showBeingBuiltProgressBar()
{
	auto background = m_beingBuildProgressBar->getParent();
	background->setVisible(true);
}

void Building::hideBeingBuiltProgressBar()
{
	auto background = m_beingBuildProgressBar->getParent();
	background->setVisible(false);
}

void Building::onConstructionComplete()
{
	hideBeingBuiltProgressBar();

	if (isSelected())
	{
		showHpBar();
	}
	cocos2d::log("construct done...");
	updateStatus(BuildingStatus::Working);
	SoundManager::getInstance()->stop(m_buildingSoundId);
}

void Building::updateStatus(BuildingStatus buildingStatus)
{
	for (auto& buildStatusSpriteIter : m_buildingStatusSpriteMap)
	{
		if (buildStatusSpriteIter.first == buildingStatus)
		{
			buildStatusSpriteIter.second->setVisible(true);

			auto buildingSize = buildStatusSpriteIter.second->getContentSize();
			setContentSize(buildingSize);
			buildStatusSpriteIter.second->setPosition(cocos2d::Vec2(buildingSize.width / 2.0f, buildingSize.height / 2.0f));

			//auto buildingTemplate = TemplateManager::getInstance()->getBuildingTemplateBy(_templateName);
			float selectedTipsYPosition = m_bottomGridsPlaneCenterPositionInLocalSpace.y;

			switch (buildingStatus)
			{
			case BuildingStatus::BeingBuilt:
			{
				if (m_forceType == ForceType::Player)
				{
					m_buildingSoundId = SoundManager::getInstance()->playBuildingEffect(BuildingSoundEffectType::Construct, true);
				}

				if (m_bottonGridPos.empty())
				{
					//initBottomGridInMapPositionList();
				}

				//updateCoveredByBuildingTileNodesGID(OBSTACLE_ID);

				hideHpBar();
				showBeingBuiltProgressBar();

				auto onUpdateToWorkingStatus = cocos2d::CallFunc::create(CC_CALLBACK_0(Building::onConstructionComplete, this));
				auto sequenceAction = cocos2d::Sequence::create(cocos2d::DelayTime::create(m_buildingTimeBySecond), onUpdateToWorkingStatus, nullptr);
				runAction(sequenceAction);

				//selectedTipsYPosition = buildingTemplate->shadowYPositionInBeingBuiltStatus;
			}
			break;
			case BuildingStatus::Working:
			{
				//如果是防御塔，则需要加一个弓箭手
				if (m_buildingType == BuildingType::DefenceTower)
				{
					FaceDirection faceDirection = FaceDirection::Invalid;
					if (m_forceType == ForceType::AI)
					{
						faceDirection = FaceDirection::FaceToWest;
					}
					else
					{
						faceDirection = FaceDirection::FaceToEast;
					}
					auto& contentSize = getContentSize();
					cocos2d::Vec2 position = cocos2d::Vec2(contentSize.width/2, contentSize.height - 50);
					m_archer = Soldier::create(m_forceType, SoldierType::Archer, position, faceDirection);
					if (m_archer != nullptr)
					{
						m_archer->setScale(1.0);
						m_archer->inDefenceTower(true);
						m_archer->showHpBar(false);
						m_archer->setAttackDistance(m_attackDistance);
						m_archer->setAttackPower(m_attackPower);
						addChild(m_archer);
						GameObjectManager::getInstance()->addGameObject(m_archer);
					}
				}
				/*_defenceNpc = createDefenceNpc(_templateName);

				if (_bottomGridInMapPositionList.empty())
				{
					initBottomGridInMapPositionList();
				}*/

				// 因为有些建筑物不需要经历建造阶段就可以直接进入working状态，因此这里需要再次更新占用格子的gid
				//updateCoveredByBuildingTileNodesGID(OBSTACLE_ID);

				//selectedTipsYPosition = buildingTemplate->shadowYPositionInWorkingStatus;
			}
			break;
			case BuildingStatus::Destory:
			{
				//建造中就被摧毁了
				if (m_buildingSoundId != 0)
				{
					SoundManager::getInstance()->stop(m_buildingSoundId);

				}
				SoundManager::getInstance()->playBuildingEffect(BuildingSoundEffectType::Destroyed);

				/*if (_defenceNpc)
				{
					removeDefenceNpc();
				}*/

				//GameWorldCallBackFunctionsManager::getInstance()->_createSpecialEffect(_destroySpecialEffectTemplateName, getPosition(), false);
				if (m_archer != nullptr)
				{
					m_archer->setVisible(false);
					GameObjectManager::getInstance()->addReadyToRemoveGameObject(m_archer->getId());
				}
				auto onReadyToRemove = cocos2d::CallFunc::create(CC_CALLBACK_0(Building::addToRemoveQueue, this));
				auto sequenceAction = cocos2d::Sequence::create(cocos2d::DelayTime::create(BUILDING_DELAY_REMOVE_TIME), onReadyToRemove, nullptr);
				runAction(sequenceAction);
			}
			break;
			default:    break;
			}

			//_selectedTips->setPosition(Vec2(buildingSize.width / 2.0f, selectedTipsYPosition));
		}
		else
		{
			buildStatusSpriteIter.second->setVisible(false);
		}
	}

	m_buildingStatus = buildingStatus;
}

void Building::addToRemoveQueue()
{
	//updateCoveredByBuildingTileNodesGID(PASSABLE_ID);
	GameObjectManager::getInstance()->addReadyToRemoveGameObject(m_uniqId);
}
